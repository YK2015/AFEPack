////////////////////////////////////////////////////////////////////////////////////////////
// main1.cpp :
//

#include <stdio.h>
#include <dlfcn.h>

#include <iostream>
#include <fstream>

#include "Geometry.h"
#include "HGeometry.h"

#define DIM 2

using namespace AFEPack;

int main(int argc, char * argv[])
{
	HGeometryTree<DIM> h_tree;
	h_tree.readEasyMesh("D");
	IrregularMesh<DIM> irregular_mesh(h_tree);
	irregular_mesh.globalRefine(3);

	do {
		irregular_mesh.semiregularize();
		irregular_mesh.regularize(false);
		RegularMesh<DIM>& regular_mesh = irregular_mesh.regularMesh();
		regular_mesh.writeOpenDXData("D.dx");
		std::cout << "Press ENTER to continue or CTRL+C to stop ..." << std::flush;
		getchar();

		Indicator<DIM> indicator(regular_mesh);
		Point<DIM> c0(0.495, 0.5);
		Point<DIM> c1(0.505, 0.5);
		for (int i = 0;i < regular_mesh.n_geometry(2);i ++) {
			Point<DIM>& p0 = regular_mesh.point(regular_mesh.geometry(2,i).vertex(0));
			Point<DIM>& p1 = regular_mesh.point(regular_mesh.geometry(2,i).vertex(1));
			Point<DIM>& p2 = regular_mesh.point(regular_mesh.geometry(2,i).vertex(2));
			Point<DIM> p((p0[0] + p1[0] + p2[0])/3., (p0[1] + p1[1] + p2[1])/3.);
			double area = (p1[0] - p0[0])*(p2[1] - p0[1])
				- (p2[0] - p0[0])*(p1[1] - p0[1]);
			double d0 = (p - c0).length();
			double d1 = (p - c1).length();
			if (d0 < 0.015 || d1 < 0.015)
				indicator[i] = area*100;
		};

		MeshAdaptor<DIM> mesh_adaptor(irregular_mesh);
		mesh_adaptor.convergenceOrder() = 0.;
		mesh_adaptor.refineStep() = 0;
		mesh_adaptor.setIndicator(indicator);
		mesh_adaptor.tolerence() = 2.5e-10;
		mesh_adaptor.adapt();
	} while (1);	
};

#undef DIM

//
// end of file
////////////////////////////////////////////////////////////////////////////////////////////
