function [err] = graphToMtx(filename)
filename
addpath(pwd);
G1 = bvgraph(filename);
A = sparse(G1);
mmwrite(append(filename,'.mtx'), A);
