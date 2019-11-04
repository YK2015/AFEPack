/**
 * @file   ex08.cpp
 * @author Ruo Li <rli@aztec>
 * @date   Tue Oct  6 11:15:27 2009
 * 
 * @brief  测试 BirdView 的 semiregularize 函数。
 * 
 * 
 */

#include "MPI_HGeometry.h"

using namespace AFEPack;

#define DIM 2
#define DOW 2

typedef AFEPack::MPI::HGeometryForest<DIM,DOW> forest_t;

int main(int argc, char * argv[])
{
  MPI_Init(&argc, &argv);

  forest_t forest(MPI_COMM_WORLD);
  forest.readMesh(argv[1]); /// 第一个参数是网格文件名

  AFEPack::MPI::BirdView<forest_t> ir_mesh(forest);
  u_int round = atoi(argv[2]); /// 第二个参数是随机加密的轮数
  for (u_int i = 0;i < round;++ i) {
    ir_mesh.randomRefine(20.0); /// 加密 5% 的单元
  }
  ir_mesh.semiregularize();
  ir_mesh.regularize();

  char filename[256];
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  sprintf(filename, "mesh%d.dx", rank);
  ir_mesh.regularMesh().writeOpenDXData(filename);

  MPI_Finalize();
  return 0;
}

/**
 * end of file
 * 
 */

