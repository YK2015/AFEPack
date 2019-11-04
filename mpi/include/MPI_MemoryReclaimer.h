/**
 * @file   MPI_MemoryReclaimer.h
 * @author Ruo Li <rli@math.pku.edu.cn>
 * @date   Wed Jan 12 09:47:48 2011
 * 
 * @brief  �˴���ר��Ϊʹ�� ULoadBalance ���˸���ƽ���Ժ������������
 *         �ڴ���յ�ʱ��ʹ�õġ�Ϊ�˱�����������Ƕ����еĹ�������
 *         ��δ���л��ա�
 * 
 */

#ifndef __MPI_MemoryReclaimer_h__
#define __MPI_MemoryReclaimer_h__

#include "MPI_HGeometry.h"

/*!

  �˴���ʹ�õ��㷨�ʹ��е� MemoryReclaimer ��һ���ģ�����֮ͬ���Ƕ��ڹ�
  �����彫������л��ա�����������ʹ�� ULoadBalance ���˸���ƽ����
  �����α�úܸ��ӣ����Ǻ���ʵ�ֹ�������Ŀ�����ͬʱ���գ���˶���
  �еĹ������嶼�����л��ա�Ϊ�˲����չ������壬�����ֱ������ṹ��
  Ҫ��

      "һ����������ĳ�������ϣ�����������ֵܶ���Ȼ�ڴ˷������п���"

  ������ͨ�����в��轫���в����յļ������϶���һ����ǣ��ں���Ĳ���
  �У����Ǽ��˱�ǣ���ȷ����Щ�����岻�ᱻ���ա�����Ϊ��

  1. ������������������Ϊ��������ĵ�Ԫ��������б�ǣ����������
     �Ǽӵ����ֵ����ϣ�

  2. �����б�ǹ��ĵ�Ԫ�������������������ϱ�ǣ�

  �����м����˱�ǵļ����壬ͨ����relabelHGeometryRecursively �����У�
  ��������� index ��Ϊ -2 ��ʱ�򣬶Լ������Ƿ��ǹ���������Ǽ�����Ĺ�
  ���ǹ���������һ���жϣ�����ǣ��Ͳ��� index �޸�Ϊ-2����� index
  ����-2��������Ͳ��ᱻ���ա�����Ļ����乹���ǹ���ļ�����Ļ��ս���
  ����ƽ��Ĳ���֮�󣬹��������Ϊ���������ļ������Ժ�Ž��С���˴�
  �����ڴ���ղ��Ƿǳ���ȫ���������ڹ�����������������٣�������Ӧ��
  ���ǿ��Խ��ܵġ�

*/

AFEPACK_OPEN_NAMESPACE

namespace MPI {
  template <class FOREST>
    class MemoryReclaimer {
  public:
    enum { dim = FOREST::dim, dow = FOREST::dow };
    typedef FOREST tree_t;
    typedef BirdView<tree_t> ir_mesh_t;
  private:
    tree_t * h_tree;
    std::list<ir_mesh_t *> ir_mesh;

  public:
  MemoryReclaimer() : h_tree(NULL) {}
  MemoryReclaimer(tree_t& _h_tree) : h_tree(&_h_tree) {}
    virtual ~MemoryReclaimer() {}

  public:
    /**
     * ���ý����ڴ���ղ�����HGeometryTree��
     * 
     */
    void setGeometryTree(tree_t& _h_tree) {
      h_tree = &_h_tree;
    }
    /**
     * �ӽ�һ���μӲ�����IrregularMesh��
     * 
     */
    void addIrregularMesh(ir_mesh_t& _ir_mesh) {
      if (&(_ir_mesh.getForest()) != h_tree) {
        std::cout << "warning: the irregular mesh added is not based on the geometry tree used."
                  << std::endl;
      }
      ir_mesh.push_back(&_ir_mesh);
    }
    void clear() {
      h_tree = NULL;
      ir_mesh.clear();
    }
    void reclaim() {
      /// �ȶԸ���IrregularMesh�Ĳ�ʹ���ڴ���л���
      typename std::list<ir_mesh_t *>::iterator 
        the_ir_mesh = ir_mesh.begin(),
        end_ir_mesh = ir_mesh.end();
      for (;the_ir_mesh != end_ir_mesh;++ the_ir_mesh) {
        reclaimIrregularMesh(**the_ir_mesh);
      }
  
      /// �Ժ͹��������йصļ��������ϱ��
      new_property_id(_pid_is_shared);
      markSharedObject();

      /// �����������־Ϊ -1
      initialTreeLabel();

      /// ��ʹ���е������ʶΪ 1
      the_ir_mesh = ir_mesh.begin();
      for (;the_ir_mesh != end_ir_mesh;++ the_ir_mesh) {
        labelIrregularMesh(**the_ir_mesh);
      }

      /// ���ڴ���л���
      reclaimTreeMemory();
      free_property_id(_pid_is_shared);
    }

  private:
    void markSharedObject() {
      typename HGeometryTree<dim,dow>::RootIterator 
        the_ele = h_tree->beginRootElement(),
        end_ele = h_tree->endRootElement();
      for (;the_ele != end_ele;++ the_ele) {
        HGeometry<dim,dow> * p_geo = &(*the_ele);
        do {
          if (p_geo->parent != NULL) {
            p_geo = p_geo->parent;
          } else break;
        } while (true);
        markSharedElement(*p_geo);
      }

      the_ele = h_tree->beginRootElement();
      for (;the_ele != end_ele;++ the_ele) {
        HGeometry<dim,dow> * p_geo = &(*the_ele);
        do {
          if (p_geo->parent != NULL) {
            p_geo = p_geo->parent;
          } else break;
        } while (true);
        markSharedGeometry(*p_geo);
      }
    }      

    void reclaimIrregularMesh(ir_mesh_t& m) {
      ActiveElementIterator<dim,dow>
        the_ele = m.beginActiveElement(),
        end_ele = m.endActiveElement();
      for (;the_ele != end_ele;++ the_ele) {
        if (the_ele->isRefined()) {
          for (int i = 0;i < the_ele->n_child;++ i) {
            m.deleteTree(the_ele->child[i]);
            the_ele->child[i] = NULL;
          }
        }
      }
    }

    void initialTreeLabel() {
      typename HGeometryTree<dim,dow>::RootIterator 
        the_ele = h_tree->beginRootElement(),
        end_ele = h_tree->endRootElement();
      for (;the_ele != end_ele;++ the_ele) {
        labelHGeometryRecursively(*the_ele, -1);
      }
    }

    void labelIrregularMesh(ir_mesh_t& m) {
      RootFirstElementIterator<dim,dow>
        the_ele = m.beginRootFirstElement(),
        end_ele = m.endRootFirstElement();
      for (;the_ele != end_ele;++ the_ele) {
        labelHGeometry(*(the_ele->h_element), 1);
      }

      typename ir_mesh_t::mesh_t& mesh = m.regularMesh();
#define LABEL_USED_HGEOMETRY(D)                                         \
      if (dim >= D) {                                                   \
        int n_geo = mesh.h_geometry()[D].size();                        \
        for (int i = 0;i < n_geo;++ i) {                                \
          HGeometry<D,dow> * p_geo = mesh.template h_geometry<D>(i);    \
          labelHGeometry(*p_geo, 1);                                    \
        }                                                               \
      }

      LABEL_USED_HGEOMETRY(0);
      LABEL_USED_HGEOMETRY(1);
      LABEL_USED_HGEOMETRY(2);
      LABEL_USED_HGEOMETRY(3);

#undef LABEL_USED_HGEOMETRY
    }

    void reclaimTreeMemory() {
      typename HGeometryTree<dim,dow>::RootIterator
        the_ele = h_tree->beginRootElement(),
        end_ele = h_tree->endRootElement();
      for (;the_ele != end_ele;++ the_ele) {
        relabelHGeometryRecursively(*the_ele);
      }

      the_ele = h_tree->beginRootElement();
      for (;the_ele != end_ele;++ the_ele) {
        reclaimHGeometryRecursively(*the_ele);
      }
    }

  private:
    property_id_t<> _pid_is_shared;

    /**
     * �ж�һ���������Ƿ��ǹ���ģ����߾��й��������
     */
    template <class GEO> bool
      is_shared_geometry(const GEO& g) const {
      bool result = false;
      result |= (h_tree->get_shared_info(g) != NULL);
      if (! result) {
        for (int i = 0;i < g.n_vertex;++ i) {
          result |= is_shared_geometry(*(g.vertex[i]));
        }
      }
      if (result) {
        return result;
      } else {
        for (int i = 0;i < g.n_boundary;++ i) {
          result |= is_shared_geometry(*(g.boundary[i]));
        }
      }
      return result;
    }

    /**
     * �Ե�Ԫ ele �ݹ��������ñ�ǡ����������Ҫ��ǣ������ֵ�Ҳ���
     * �ϣ�Ȼ��������ݹ���С�
     */
    void markSharedElement(HGeometry<dim,dow>& ele) const {
      if ((ele.get_property(_pid_is_shared) == NULL) &&
          (is_shared_geometry(ele))) {
        ele.new_property(_pid_is_shared);

        if (ele.parent != NULL) {
          for (int i = 0;i < ele.parent->n_child;++ i) {
            HGeometry<dim,dow> * p_sibling = ele.parent->child[i];
            if (p_sibling == &ele) continue;
            if (p_sibling->get_property(_pid_is_shared) == NULL) {
              p_sibling->new_property(_pid_is_shared);
            }
          }
        }
      }
      
      if (ele.isRefined()) {
        for (int i = 0;i < ele.n_child;++ i) {
          markSharedElement(*ele.child[i]);
        }
      }
    }

    /**
     * ����������Ѿ�����ǣ��������ж���ͱ߽��ǣ����Ա߽�ݹ��
     * �б�ǡ�Ȼ�������ĺ���ݹ���С�
     */
    template <class GEO> void
      markSharedGeometry(GEO& g) const {
      if (g.get_property(_pid_is_shared) != NULL) {
        for (int i = 0;i < g.n_vertex;++ i) {
          if (g.vertex[i]->get_property(_pid_is_shared) == NULL) {
            g.vertex[i]->new_property(_pid_is_shared);
          }
        }
        for (int i = 0;i < g.n_boundary;++ i) {
          if (g.boundary[i]->get_property(_pid_is_shared) == NULL) {
            g.boundary[i]->new_property(_pid_is_shared);
          }
          markSharedGeometry(*g.boundary[i]);
        }
      }
      if (g.isRefined()) {
        for (int i = 0;i < g.n_child;++ i) {
          markSharedGeometry(*g.child[i]);
        }
      }
    }      

    template <class GEO> void 
      labelHGeometry(GEO& g, int lab) const {
      for (int i = 0;i < g.n_vertex;++ i) {
        labelHGeometry(*(g.vertex[i]), lab);
      }
      for (int i = 0;i < g.n_boundary;++ i) {
        labelHGeometry(*(g.boundary[i]), lab);
      }
      g.index = lab;
    }

    template <class GEO> void 
      labelHGeometryRecursively(GEO& g, int lab) const {
      labelHGeometry(g, lab);

      if (g.isRefined()) {
        for (int i = 0;i < g.n_child;++ i) {
          labelHGeometryRecursively(*(g.child[i]), lab);
        }
      }
    }

    template <class GEO> int 
      relabelHGeometryRecursively(GEO& g) const {
      if (g.get_property(_pid_is_shared) != NULL) {
        g.index = 1;
      }

      for (int i = 0;i < g.n_vertex;++ i) {
        if (g.vertex[i] == NULL) continue;
        if (relabelHGeometryRecursively(*(g.vertex[i])) == -2) {
          g.vertex[i] = NULL;
        }
      }
      for (int i = 0;i < g.n_boundary;++ i) {
        if (g.boundary[i] == NULL) continue;
        if (relabelHGeometryRecursively(*(g.boundary[i])) == -2) {
          g.boundary[i] = NULL;
        }
      }
      if (g.isRefined()) {
        for (int i = 0;i < g.n_child;++ i) {
          if (g.child[i] == NULL) continue;
          if (relabelHGeometryRecursively(*(g.child[i])) == -2) {
            g.child[i] = NULL;
          }
        }
      }
      if (g.index == -1) {
        g.index = -2;
        return -1;
      } else {
        return g.index;
      }
    }

    template <class GEO> int 
      reclaimHGeometryRecursively(GEO& g) const {
      for (int i = 0;i < g.n_vertex;++ i) {
        if (g.vertex[i] != NULL) {
          if (reclaimHGeometryRecursively(*(g.vertex[i])) == -1) {
            g.vertex[i] = NULL;
          }
        }
      }
      for (int i = 0;i < g.n_boundary;++ i) {
        if (g.boundary[i] != NULL) {
          if (reclaimHGeometryRecursively(*(g.boundary[i])) == -1) {
            g.boundary[i] = NULL;
          }
        }
      }
      for (int i = 0;i < g.n_child;++ i) {
        if (g.child[i] == NULL) continue;
        if (reclaimHGeometryRecursively(*(g.child[i])) == -1) {
          g.child[i] = NULL;
        }
      }

      if ((g.index == -2) && 
          (g.get_property(_pid_is_shared) == NULL)) {
        this->reclaimHGeometry(&g);
        return -1;
      } else {
        return 1;
      }
    }

    template <class GEO> void 
      reclaimHGeometry(GEO * p_geo) const {
      assert ((p_geo->get_property(_pid_is_shared) == NULL));
      delete p_geo;
    }
  };

}

AFEPACK_CLOSE_NAMESPACE

#endif // __MPI_MemoryReclaimer_h__

/**
 * end of file
 * 
 */
