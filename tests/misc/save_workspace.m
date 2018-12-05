function save_workspace(filepath)
global FINISH PARAMS PrC PrP STATE
global PARAMS STATE CYCLES
global P_EVO POP GEN LIB

save('temp.mat')
json = jsonencode(load('temp.mat'));
id1 = 1;
fid = fopen(filepath, 'wt');
fprintf(fid, json);
fclose(fid);
