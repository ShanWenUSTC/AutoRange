function [nodesnum, posmat, indexmat, typevec] = ReadNodes(filename)
%
%
%

fid = fopen(filename, 'r')

nodesnum = fscanf(fid, '%d', 1)

posmat = fscanf(fid, '%f', [2, nodesnum])'

indexmat = fscanf(fid, '%d', [2, nodesnum])'

typevec = fscanf(fid, '%d', nodesnum)

fclose(fid);
end