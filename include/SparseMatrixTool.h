/**
 * @file   SparseMatrixTool.h
 * @author Robert Lie
 * @date   Sun Nov  4 09:06:11 2007
 * 
 * @brief  
 * 
 * 
 */


#include "Miscellaneous.h"

/**
 * ����һЩ��ϡ������ϡ�����ģ����в����Ĺ��ߺ���
 * 
 */
namespace SparseMatrixTool {

  /** 
   * ������ϡ�����ģ�尴ˮƽ�������ӳ�Ϊһ���µ�ģ��
   * Ҫ�����������������ͬ��������
   * 
   * @param sp0 ��ߵ�ģ��
   * @param sp1 �ұߵ�ģ��
   * @param sp �õ�����ģ��
   */
  void hCatSparsityPattern(const SparsityPatternBase<>& sp0,
			   const SparsityPatternBase<>& sp1,
			   SparsityPatternBase<>& sp); 

  /**
   * ������ϡ�����ģ�尴��ֱ�������ӳ�Ϊһ���µ�ģ��
   * Ҫ�����������������ͬ��������
   *
   * @param sp0 �ϱߵ�ģ��
   * @param sp1 �±ߵ�ģ��
   * @param sp �õ�����ģ��
   */
  void vCatSparsityPattern(const SparsityPatternBase<>& sp0,
			   const SparsityPatternBase<>& sp1,
			   SparsityPatternBase<>& sp);

  /**
   * ������ϡ�����ģ�尴��������ķ�ʽ���ӳ�Ϊһ���µ�
   * ģ�壬Ҳ���Ƕ��ڸ�����ϡ����� \f$ A \f$ �� \f$ B
   * \f$�����ǵõ�����
   *
   * \f[
   *    \left(\begin{array}{cc}
   *      A & B \\
   *      B^T & 0
   *    \end{array}\right)
   * \f]
   *
   * ��ģ�塣Ҫ�� \f$ A \f$ �� \f$ B \f$ ������ͬ������
   * �Լ� \f$ A \f$ ��һ������
   * 
   * @param sp0 ���� \f$ A \f$ ��ģ��
   * @param sp1 ���� \f$ B \f$ ��ģ��
   * @param sp �õ�����ģ��
   */
  void gammaCatSparsityPattern(const SparsityPatternBase<>& sp0,
			       const SparsityPatternBase<>& sp1,
			       SparsityPatternBase<>& sp);

  /**
   * ������ϡ�����ģ�尴�նԽǷ�ʽ���ӳ�Ϊһ���µ�ģ�塣
   * 
   * @param sp0 ���ϽǾ����ģ��
   * @param sp1 ���½Ǿ����ģ��
   * @param sp �õ�����ģ��
   */
  void dCatSparsityPattern(const SparsityPatternBase<>& sp0,
			   const SparsityPatternBase<>& sp1,
			   SparsityPatternBase<>& sp);

  /** 
   * ���ĸ�ϡ�����ģ����ȫ����Ϊһ���µ�ģ��
   *
   * \f[
   *    \left(\begin{array}{cc}
   *      A_{00} & A_{01} \\
   *      A_{01} & A_{11}
   *    \end{array}\right)
   * \f]
   * 
   * @param sp00 ���ϽǾ����ģ��
   * @param sp01 ���ϽǾ����ģ��
   * @param sp10 ���½Ǿ����ģ��
   * @param sp11 ���½Ǿ����ģ��
   * @param sp �õ�����ģ��
   */  
  void fullCatSparsityPattern(const SparsityPatternBase<>& sp00,
			      const SparsityPatternBase<>& sp01,
			      const SparsityPatternBase<>& sp10,
			      const SparsityPatternBase<>& sp11,
			      SparsityPatternBase<>& sp);

  /**
   * ������ϡ�������ˮƽ��ʽ���ӳ�Ϊһ���µ�ϡ�����
   * 
   * @param m0 ��ߵľ���
   * @param m1 �ұߵľ���
   * @param sp �õ����¾����ģ��
   * @param m �õ����¾���
   * @param is_pattern_ok ���Ϊ�棬���ģ����й��죻����
   *                      �ٶ�ģ���Ѿ���������ȷ�Ĺ��죻ȱ
   *                      ʡֵΪ�档
   */
  template <typename number>
    void hCatSparseMatrix(const SparseMatrix<number>& m0,
			  const SparseMatrix<number>& m1,
			  SparsityPatternBase<>& sp,
			  SparseMatrix<number>& m,
			  bool is_pattern_ok = true);

  /**
   * ������ϡ���������ֱ��ʽ���ӳ�Ϊһ���µ�ϡ�����
   * 
   * @param m0 �ϱߵľ���
   * @param m1 �±ߵľ���
   * @param sp �õ����¾����ģ��
   * @param m �õ����¾���
   * @param is_pattern_ok ���Ϊ�棬���ģ����й��죻����
   *                      �ٶ�ģ���Ѿ���������ȷ�Ĺ��죻ȱ
   *                      ʡֵΪ�档
   */
  template <typename number>
    void vCatSparseMatrix(const SparseMatrix<number>& m0,
			  const SparseMatrix<number>& m1,
			  SparsityPatternBase<>& sp,
			  SparseMatrix<number>& m,
			  bool is_pattern_ok = true);

  /**
   * ������ϡ����󰴰�������ķ�ʽ���ӳ�Ϊһ���µ�ϡ��
   * ����Ҳ���Ƕ��ڸ�����ϡ����� \f$ A \f$ �� \f$ B
   * \f$�����ǵõ�����
   *
   * \f[
   *    \left(\begin{array}{cc}
   *      A & B \\
   *      B^T & 0
   *    \end{array}\right)
   * \f]
   *
   * @param m0 ���� \f$ A \f$
   * @param m1 ���� \f$ B \f$
   * @param sp �õ����¾����ģ��
   * @param m �õ����¾���
   * @param is_pattern_ok ���Ϊ�棬���ģ����й��죻����
   *                      �ٶ�ģ���Ѿ���������ȷ�Ĺ��죻ȱ
   *                      ʡֵΪ�档
   */
  template <typename number>
    void gammaCatSparseMatrix(const SparseMatrix<number>& m0,
			      const SparseMatrix<number>& m1,
			      SparsityPatternBase<>& sp,
			      SparseMatrix<number>& m,
			      bool is_pattern_ok = true);

  /**
   * ������ϡ������նԽǷ�ʽ���ӳ�Ϊһ���µ�ϡ�����
   * 
   * @param m0 ���ϽǾ���
   * @param m1 ���½Ǿ���
   * @param sp �õ����¾����ģ��
   * @param m �õ����¾���
   * @param is_pattern_ok ���Ϊ�棬���ģ����й��죻����
   *                      �ٶ�ģ���Ѿ���������ȷ�Ĺ��죻ȱ
   *                      ʡֵΪ�档
   */
  template <typename number>
    void dCatSparseMatrix(const SparseMatrix<number>& m0,
			  const SparseMatrix<number>& m1,
			  SparsityPatternBase<>& sp,
			  SparseMatrix<number>& m,
			  bool is_pattern_ok = true);

  /** 
   * ���ĸ�ϡ�������ȫ����Ϊһ���µ�ϡ�����
   *
   * \f[
   *    \left(\begin{array}{cc}
   *      A_{00} & A_{01} \\
   *      A_{10} & A_{11}
   *    \end{array}\right)
   * \f]
   * 
   * @param m00 ���ϽǾ���
   * @param m01 ���ϽǾ���
   * @param m10 ���½Ǿ���
   * @param m11 ���½Ǿ���
   * @param sp �õ����¾����ģ��
   * @param m �õ����¾���
   * @param is_pattern_ok ���Ϊ�棬���ģ����й��죻����
   *                      �ٶ�ģ���Ѿ���������ȷ�Ĺ��죻ȱ
   *                      ʡֵΪ�档
   */  
  template <typename number>
    void fullCatSparseMatrix(const SparseMatrix<number>& m00,
			     const SparseMatrix<number>& m01,
			     const SparseMatrix<number>& m10,
			     const SparseMatrix<number>& m11,
			     SparsityPatternBase<>& sp,
			     SparseMatrix<number>& m,
			     bool is_pattern_ok = true);

  template <typename number>
    void SparseMatrixTranspose(const SparseMatrix<number>& m,
        SparsityPatternBase<>& sp, SparseMatrix<number>& mt){
      std::vector<int> max_col(m.n(),0);
      auto begin = m.begin();
      auto end = m.end();
      int r,c;
      number val;
      for(;begin != end; ++begin){
        c = (*begin).column();
        max_col[c] += 1;
      }
      int num_max_col = *std::max_element(max_col.begin(),
          max_col.end());
      sp.reinit(m.n(),m.m(),num_max_col);
      begin = m.begin();
      for(;begin != end; ++begin){
        r = (*begin).row();
        c = (*begin).column();
        sp.add(c,r);
      }

      begin = m.begin();
      mt.reinit(sp);
      for(;begin != end; ++begin){
        r = (*begin).row();
        c = (*begin).column();
        val = (*begin).value();
        mt.add(c,r,val);
      }
    }



};

/**
 * end of file
 * 
 */
