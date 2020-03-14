#This makefile compiles all codes to make running times faster
#if you edit a single file, just that one file needs to be recompiled

MoM3D: globalVars.cpp MoM3D.cpp trimesh3D.cpp triplot.cpp edgemakeMoM.cpp outside_edge.cpp renumber_RWG.cpp edge_conx_elem.cpp find_local_dofs.cpp compute_rho_c.cpp fill_z_by_edge.cpp integral_pq.cpp tri_area3D.cpp intg_sing_SGF.cpp simplex_area.cpp GL_quad_rule.cpp tri_quad.cpp fill_v_vector.cpp post_proc_mom.cpp
	g++ -I ./Eigen/ globalVars.cpp MoM3D.cpp trimesh3D.cpp triplot.cpp edgemakeMoM.cpp outside_edge.cpp renumber_RWG.cpp edge_conx_elem.cpp find_local_dofs.cpp compute_rho_c.cpp fill_z_by_edge.cpp integral_pq.cpp tri_area3D.cpp intg_sing_SGF.cpp simplex_area.cpp GL_quad_rule.cpp tri_quad.cpp fill_v_vector.cpp post_proc_mom.cpp

#^This is bad practice tho.
