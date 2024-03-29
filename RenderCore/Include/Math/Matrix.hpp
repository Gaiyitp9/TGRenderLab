﻿/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename ScalarT, int _Rows, int _Cols, StorageOption Option>
	struct traits<Matrix<ScalarT, _Rows, _Cols, Option>>
	{
		using Scalar = ScalarT;
		constexpr static int	Rows = _Rows;
		constexpr static int	Cols = _Cols;
		constexpr static bool	IsRowMajor = Option == StorageOption::RowMajor;
		constexpr static bool	IsVector = Rows == 1 || Cols == 1;
		constexpr static bool	IsSquare = Rows == Cols;
		constexpr static int	Size = Rows * Cols;
	};

	template<typename Scalar, int Rows, int Cols, StorageOption Option>
	class Matrix
	{
	public:
		Matrix() { std::memset(m_storage.data(), 0, m_storage.size() * sizeof(Scalar)); }
		Matrix(const Matrix& other) : m_storage(other.m_storage) {}
		Matrix(Scalar x, Scalar y)
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime &&
				traits<Matrix>::SizeAtCompileTime == 2, "只有二维向量可以使用这个构造函数");
			m_storage[0] = x;
			m_storage[1] = y;
		}
		Matrix(Scalar x, Scalar y, Scalar z)
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime &&
				traits<Matrix>::SizeAtCompileTime == 3, "只有三维向量可以使用这个构造函数");
			m_storage[0] = x;
			m_storage[1] = y;
			m_storage[2] = z;
		}
		Matrix(Scalar x, Scalar y, Scalar z, Scalar w)
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime &&
				traits<Matrix>::SizeAtCompileTime == 4, "只有四维向量可以使用这个构造函数");
			m_storage[0] = x;
			m_storage[1] = y;
			m_storage[2] = z;
			m_storage[3] = w;
		}

		template<typename OtherDerived>
		Matrix& operator=(const Matrix& other)
		{
			AssignmentOp<Matrix>::Run(*this, other);
			return *this;
		}

	public:
		constexpr int rows() const { return m_storage.rows(); }
		constexpr int cols() const { return m_storage.cols(); }
		constexpr int size() const { return m_storage.size(); }
		const Scalar* data() const { return m_storage.data(); }
		Scalar* data() { return m_storage.data(); }

		static Matrix identity()
		{
			static_assert(!traits<Matrix>::IsDynamic && 
				traits<Matrix>::IsSquareAtCompileTime, "Only static and square matrix has identity.");
			Matrix identityMat;
			for (int i = 0; i < traits<Matrix>::RowsAtCompileTime; ++i)
				identityMat(i, i) = static_cast<Scalar>(1.0);
			return identityMat;
		}

		const Scalar& x() const
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "x() is only for vectors.");
			return m_storage[0];
		}
		const Scalar& y() const
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "y() is only for vectors.");
			return m_storage[1];
		}
		const Scalar& z() const
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "z() is only for vectors.");
			return m_storage[2];
		}
		const Scalar& w() const
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "w() is only for vectors.");
			return m_storage[3];
		}

		Scalar& x()
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "x() is only for vectors.");
			return m_storage[0];
		}
		Scalar& y()
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "y() is only for vectors.");
			return m_storage[1];
		}
		Scalar& z()
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "z() is only for vectors.");
			return m_storage[2];
		}
		Scalar& w()
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "w() is only for vectors.");
			return m_storage[3];
		}

		const Scalar& operator[](int index) const
		{
			return m_storage[index];
		}
		const Scalar& operator()(int row, int col) const
		{
			if constexpr (traits<Matrix>::IsRowMajor)
				return m_storage[col + row * m_storage.cols()];
			else
				return m_storage[row + col * m_storage.rows()];
		}
		Scalar& operator[](int index)
		{
			return m_storage[index];
		}
		Scalar& operator()(int row, int col)
		{
			if constexpr (traits<Matrix>::IsRowMajor)
				return m_storage[col + row * m_storage.cols()];
			else
				return m_storage[row + col * m_storage.rows()];
		}

		Scalar magnitude() const
		{
			if constexpr (!traits<Matrix>::IsDynamic)
				static_assert(traits<Matrix>::IsVectorAtCompileTime, "Magnitude is only for vectors.");
			else if (rows() != 1 && cols() != 1)
				Debug::LogLine(L"magnitude is only for vectors.");

			return sqrt(Dot(*this));
		}

		Scalar sqrMagnitude() const
		{
			if constexpr (!traits<Matrix>::IsDynamic)
				static_assert(traits<Matrix>::IsVectorAtCompileTime, "sqrMagnitude is only for vectors.");
			else if (rows() != 1 && cols() != 1)
				Debug::LogLine(L"sqrMagnitude is only for vectors.");

			return Dot(*this);
		}

		Matrix normalized() const
		{
			if constexpr (!traits<Matrix>::IsDynamic)
				static_assert(traits<Matrix>::IsVectorAtCompileTime, "sqrMagnitude is only for vectors.");
			else if (rows() != 1 && cols() != 1)
				Debug::LogLine(L"sqrMagnitude is only for vectors.");

			Scalar mag = magnitude();
			if (Abs(mag) < NumTraits<Scalar>::Epsilon)
				return *this;
			else
				return *this / mag;
		}

		// TODO: n维矩阵行列式
		Scalar determinant() const
		{
			if constexpr (!traits<Matrix>::IsDynamic)
				static_assert(traits<Matrix>::IsSquareAtCompileTime, "determinant is only for square matrix.");
			else if (rows() != cols())
				Debug::LogLine(L"determinant is only for square matrix.");

			return 0;
		}

		// 矩阵转置
		Matrix transpose() const
		{
			Matrix<Scalar, Cols, Rows, Option> t;
			for (int i = 0; i < rows(); ++i)
			{
				for (int j = 0; j < cols(); ++i)
				{
					t(j, i) = (*this)(i, j);
				}
			}
			return t;
		}

		// 矩阵的逆
		Matrix inverse() const
		{

		}

	public:
		Matrix operator+(const Matrix& other) const
		{
			return CwiseBinaryOp<Matrix, ScalarSumOp<Scalar>>::Run(*this, other);
		}

		Matrix operator-(const Matrix& other) const
		{
			return CwiseBinaryOp<Matrix, ScalarSubOp<Scalar>>::Run(*this, other);
		}

		Matrix operator/(const Scalar& factor) const
		{
			if (Abs(factor) < NumTraits<Scalar>::Epsilon)
				return *this;

			Matrix factorMatrix;
			for (int i = 0; i < rows(); ++i)
			{
				for (int j = 0; j < cols(); ++j)
				{
					factorMatrix(i, j) = factor;
				}
			}
			return CwiseBinaryOp<Matrix, ScalarDivideOp<Scalar>>::Run(*this, factorMatrix);
		}

		Matrix operator*(const Scalar& factor) const
		{
			Matrix factorMatrix;
			for (int i = 0; i < rows(); ++i)
			{
				for (int j = 0; j < cols(); ++j)
				{
					factorMatrix(i, j) = factor;
				}
			}
			return CwiseBinaryOp<Matrix, ScalarProductOp<Scalar>>::Run(*this, factorMatrix);
		}

		// 矩阵乘法(注意满足矩阵的列与other矩阵的行数要相等)
		template<typename OtherMatrix> 
		typename MatricesMultipleTraits<Matrix, OtherMatrix>::ReturnType 
			operator*(const OtherMatrix& other) const
		{
			return Multiple<Matrix, OtherMatrix>::Run(*this, other);
		}

		// 逐元素乘法
		Matrix CWiseMultiple(const Matrix& other) const
		{
			return CwiseBinaryOp<Matrix, ScalarProductOp<Scalar>>::Run(*this, other);
		}

		Scalar Dot(const Matrix& other) const
		{
			return DotOp<Matrix>::Run(*this, other);
		}

		// 向量叉乘
		// 暂时计算三维向量叉乘。TODO: n维向量叉乘
		Matrix Cross(const Matrix& other) const
		{
			if constexpr (!traits<Matrix>::IsDynamic)
			{
				static_assert(traits<Matrix>::IsVectorAtCompileTime 
					&& traits<Matrix>::SizeAtCompileTime > 2, "Cross is only for 3d and above vectors.");
			}
			else if ((rows() != 1 && cols() != 1) || size() <= 2)
				Debug::LogLine(L"Cross is only for 3d and above vectors.");

			Matrix cross;
			cross[0] = m_storage[1] * other[2] - m_storage[2] * other[1];
			cross[1] = m_storage[2] * other[0] - m_storage[0] * other[2];
			cross[2] = m_storage[0] * other[1] - m_storage[1] * other[0];
			return cross;
		}

		void Normalize()
		{
			Scalar mag = magnitude();
			if (Abs(mag) < NumTraits<Scalar>::Epsilon)
				return;

			*this = *this / mag;
		}

	private:
		Scalar m_storage[4];
	};

	// 矩阵乘以一个系数
	template<typename Scalar, int Rows, int Cols, StorageOption Option>
	Matrix<Scalar, Rows, Cols, Option> operator*(const Scalar& factor, const Matrix<Scalar, Rows, Cols, Option>& matrix)
	{
		return matrix * factor;
	}

	template<typename Scalar, int Size>
	using Vector = Matrix<Scalar, Size, 1>;
	template<typename Scalar, int Size>
	using RowVector = Matrix<Scalar, 1, Size>;

	#define MATRIX_TYPEDEF(Type, TypeSuffix, Size, SizeSuffix)			\
	using Vector##SizeSuffix##TypeSuffix	= Vector<Type, Size>;		\
	using RowVector##SizeSuffix##TypeSuffix = RowVector<Type, Size>;	\
	using Matrix##SizeSuffix##TypeSuffix	= Matrix<Type, Size, Size>;

	#define MATRIX_ALLSIZE_TYPEDEF(Type, TypeSuffix)	\
	MATRIX_TYPEDEF(Type, TypeSuffix, 2, 2)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 3, 3)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 4, 4)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, DYNAMIC, X)		

	MATRIX_ALLSIZE_TYPEDEF(float, f)
	MATRIX_ALLSIZE_TYPEDEF(double, d)
	MATRIX_ALLSIZE_TYPEDEF(int, i)
}