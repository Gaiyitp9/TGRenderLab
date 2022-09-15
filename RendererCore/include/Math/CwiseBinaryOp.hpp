/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

template<typename BinaryOp, typename Lhs, typename Rhs>
struct traits<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>
{
	using Ancestor = remove_all_t<Lhs>;
	using Scalar = invoke_result_of<BinaryOp, const typename Lhs::Scalar&, const typename Rhs::Scalar&>::type;
	constexpr static int RowsAtCompileTime = traits<Ancestor>::RowsAtCompileTime;
	constexpr static int ColsAtCompileTime = traits<Ancestor>::ColsAtCompileTime;
	constexpr static Flag Flags = traits<Ancestor>::Flags & Flag::RowMajor;
};

// 二元运算表达式
template<typename BinaryOp, typename LhsType, typename RhsType>
class CwiseBinaryOp : public MatrixBase<CwiseBinaryOp<BinaryOp, LhsType, RhsType>>
{
public:
	using Functor	= remove_all_t<BinaryOp>;
	using LhsPlain	= remove_all_t<LhsType>;
	using RhsPlain	= remove_all_t<RhsType>;
	using LhsNested = ref_selector<LhsPlain>::type;
	using RhsNested = ref_selector<RhsPlain>::type;
	using result_type = Functor::result_type;

	static_assert(have_same_matrix_size<LhsPlain, RhsPlain>, "You mixed matrices of different sizes.");

public:
	CwiseBinaryOp(const LhsPlain& lhs, const RhsPlain& rhs, const Functor& functor = Functor())
			: m_lhs(lhs), m_rhs(rhs), m_functor(functor)
	{}

	CwiseBinaryOp(const CwiseBinaryOp<BinaryOp, LhsType, RhsType>&) = default;

	constexpr int rows() const
	{
		return traits<LhsPlain>::RowsAtCompileTime == Dynamic ? m_rhs.rows() : m_lhs.rows();
	}
	constexpr int cols() const
	{
		return traits<LhsPlain>::ColsAtCompileTime == Dynamic ? m_rhs.cols() : m_lhs.cols();
	}
	const LhsPlain& lhs() const { return m_lhs; }
	const RhsPlain& rhs() const { return m_rhs; }
	const Functor& functor() const { return m_functor; }

private:
	const Functor m_functor;
	LhsNested m_lhs;
	RhsNested m_rhs;
};

}