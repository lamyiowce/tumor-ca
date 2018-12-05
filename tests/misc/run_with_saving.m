setup
        tau = 0.8;
seed = 1;
clear global
global P_EVO POP GEN LIB

% // load runfile
evalfile('example/runfile_example_static.txt')		% provides P_EVO

% // Define the random seed, and control for replication
%    .. we use MATLAB's 'mt19937ar' stream ("Mersenne
%    Twister with Mersenne prime 2^19937-1")
s = RandStream('mt19937ar','Seed',seed);
RandStream.setGlobalStream(s);
P_EVO.randstreamseed = s;

% // load Benchmark performance for fitness comparison
BM = GetBenchmarks(P_EVO);

% // Build initial population
GetInitialPop		        % makes POP
res  = InitializeEvoRun;
RunStats = InitializeRunStats(P_EVO);		    % initialise output, produces RunStats

        tr = 1;
sl = 1;
gn = 1;

global PARAMS STATE CYCLES
ImportTumorState(sl,tr,P_EVO,POP);

global FINISH PARAMS PrC PrP STATE

FINISH = 0;
tumor_name = sprintf('out-%s-tr%.0f-',P_EVO.tum_lib_s,tr);
save_workspace(strcat(tumor_name, 'initial.json'));
ReplenishSubstrate2
save_workspace(strcat(tumor_name, 'ReplenishSubstrate.json'));

for st = 1:PARAMS.n_st
        Diffusion2
save_workspace(strcat(tumor_name, 'Diffusion.json'));
IrradiateTumor_evo(st,tau)
save_workspace(strcat(tumor_name, 'IrradiateTumor.json'));
SetLocalStates
save_workspace(strcat(tumor_name, 'SetLocalStates.json'));
MetaboliseNutrients
save_workspace(strcat(tumor_name, 'MetaboliseNutrients.json'));
SetGlobalStates
save_workspace(strcat(tumor_name, 'SetGlobalStates.json'));
RepairCells
save_workspace(strcat(tumor_name, 'RepairCells.json'));
CellDivision2
save_workspace(strcat(tumor_name, 'CellDivision.json'));
RunStats = GetStats2(st, P_EVO, RunStats);    % update RunStats
save_workspace(strcat(tumor_name, 'RunStats.json'));
FinishCheck
save_workspace(strcat(tumor_name, 'FinishCheck.json'));
if FINISH, return; end
        end

quit
