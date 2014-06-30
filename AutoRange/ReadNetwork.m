function [netmat, posmat, typevec] = ReadNetwork(filename)
%
%
%

fid = fopen(filename, 'r');
fid
nodesnum = fscanf(fid, '%d', 1);
netmat = zeros(nodesnum, nodesnum);
posmat = zeros(nodesnum, 2);
typevec = zeros(nodesnum);

netmat = fscanf(fid, '%d', [nodesnum, nodesnum]);
%for i=1:nodesnum
%    for j=1:nodesnum
%        netmat(i,j) = fscanf(fid, '%d', 1);
%    end
%end
netmat

posmat = fscanf(fid, '%d', [2, nodesnum])';
%for i=1:nodesnum
%    posmat(i,:) = fscanf(fid, '%d %d', 2);
%end
posmat

typevec = fscanf(fid, '%d', [nodesnum, 1]);
%for i=1:nodesnum
%    typevec(i) = fscanf(fid, '%d', 1);
%end

max_lines = fscanf(fid, '%d', 1);
distance_inline = fscanf(fid, '%d', 1);
distance_adjacent = fscanf(fid, '%d', 1);
distance_cross = fscanf(fid, '%d', 1);

fprintf('max_lines: %d\n', max_lines);
fprintf('distance_inline: %d\n', distance_inline);
fprintf('distance_adjacent: %d\n', distance_adjacent);
fprintf('distance_cross: %d\n', distance_cross);

fclose(fid);
netgraph = biograph(netmat);
dolayout(netgraph);
for i=1:nodesnum
    netgraph.nodes(i).Position = posmat(i,:);
    if typevec(i)==1
        netgraph.nodes(i).Color = [1 0 0];
    else
        netgraph.nodes(i).Color = [0 1 0];
    end
end

dolayout(netgraph, 'PathsOnly', true);
h = view(netgraph);
end
