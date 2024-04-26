/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    // 赋值运算
    template<typename Scalar>
    struct AssignOp
    {
        void AssignCoefficient(Scalar& a, Scalar b) { a = b; }
    };

	// 直接展开赋值
    // 通过行列赋值
    template<typename Kernel, std::size_t Index, std::size_t Stop>
    struct UnrollDefaultAssign
    {
        static constexpr std::size_t Row = Index / Traits<typename Kernel::DstXpr>::Columns;
        static constexpr std::size_t Column = Index % Traits<typename Kernel::DstXpr>::Columns;

        static void Run(Kernel& kernel)
        {
            kernel.AssignCoefficient(Row, Column);
            UnrollDefaultAssign<Kernel, Index + 1, Stop>::Run(kernel);
        }
    };
    template<typename Kernel, std::size_t Stop>
    struct UnrollDefaultAssign<Kernel, Stop, Stop>
    {
        static void Run(Kernel&){}
    };

    // 线性赋值
	template<typename Kernel, std::size_t Index, std::size_t Stop>
	struct UnrollLinearAssign
	{
		static void Run(Kernel& kernel)
		{
            kernel.AssignCoefficient(Index);
            UnrollLinearAssign<Kernel, Index + 1, Stop>::Run(kernel);
		}
	};
	template<typename Kernel, std::size_t Stop>
	struct UnrollLinearAssign<Kernel, Stop, Stop>
	{
		static void Run(Kernel&){}
	};

    // 赋值核，封装赋值过程涉及的细节
    template<typename DstEvaluator, typename SrcEvaluator, typename Functor>
    class AssignmentKernel
    {
    public:
        using DstXpr = DstEvaluator::XprType;
        using SrcXpr = SrcEvaluator::XprType;

        AssignmentKernel(DstEvaluator& dstEvaluator, const SrcEvaluator& srcEvaluator, Functor functor)
            : m_dstEvaluator(dstEvaluator), m_srcEvaluator(srcEvaluator), m_functor(functor) {}

        void AssignCoefficient(std::size_t index)
        {
            m_functor.AssignCoefficient(m_dstEvaluator.CoefficientRef(index), m_srcEvaluator.Coefficient(index));
        }

        void AssignCoefficient(std::size_t row, std::size_t column)
        {
            m_functor.AssignCoefficient(m_dstEvaluator.CoefficientRef(row, column), m_srcEvaluator.Coefficient(row, column));
        }

    private:
        DstEvaluator& m_dstEvaluator;
        const SrcEvaluator& m_srcEvaluator;
        Functor m_functor;
    };

    template<typename Kernel, TraversalType Traversal =
            CheckFlag<typename Kernel::DstXpr, typename Kernel::SrcXpr>(XprFlag::LinearAccess) ?
            TraversalType::Linear : TraversalType::Default>
    struct Assignment;

    template<typename Kernel>
    struct Assignment<Kernel, TraversalType::Default>
    {
        static void Run(Kernel& kernel)
        {
            UnrollDefaultAssign<Kernel, 0, Traits<typename Kernel::DstXpr>::Size>::Run(kernel);
        }
    };

	template<typename Kernel>
	struct Assignment<Kernel, TraversalType::Linear>
	{
		static void Run(Kernel& kernel)
		{
            UnrollLinearAssign<Kernel, 0, Traits<typename Kernel::DstXpr>::Size>::Run(kernel);
		}
	};

    // 矩阵表达式赋值概念，目标表达式和源表达式的行列数需要相等，且表达式需要是左值
    template<typename Dst, typename Src>
    concept Assignable = MatrixExpression<Dst> && MatrixExpression<Src> &&
            (Traits<Dst>::Rows == Traits<Src>::Rows) && (Traits<Dst>::Columns == Traits<Src>::Columns) &&
            CheckFlag<Dst>(XprFlag::LeftValue);

    template<typename Dst, typename Src> requires Assignable<Dst, Src>
    void CallAssignmentNoAlias(Dst& dst, const Src& src)
    {
        using DstEvaluator = Evaluator<Dst>;
        using SrcEvaluator = Evaluator<Src>;
        using AssignOp = AssignOp<typename Traits<Dst>::Scalar>;
        using Kernel = AssignmentKernel<DstEvaluator, SrcEvaluator, AssignOp>;

        DstEvaluator dstEvaluator{dst};
        SrcEvaluator srcEvaluator{src};

        Kernel kernel(dstEvaluator, srcEvaluator, AssignOp{});
        Assignment<Kernel>::Run(kernel);
    }

    template<typename Dst, typename Src> requires Assignable<Dst, Src>
    void CallAssignment(Dst& dst, const Src& src)
    {
        typename PlainMatrixType<Src>::Type temp(src);
        CallAssignmentNoAlias(dst, temp);
    }
}
