setup
        tau = 0.8;
seed = 1;
clear global
global P_EVO POP GEN LIB

% // load runfile
evalfile('example/runfile_example_static.txt')      % provides P_EVO

% // Define the random seed, and control for replication
%    .. we use MATLAB's 'mt19937ar' stream ("Mersenne
%    Twister with Mersenne prime 2^19937-1")
s = RandStream('mt19937ar','Seed',seed);
RandStream.setGlobalStream(s);
P_EVO.randstreamseed = s;

% // load Benchmark performance for fitness comparison
BM = GetBenchmarks(P_EVO);

% // Build initial population
GetInitialPop              % makes POP
res  = InitializeEvoRun;
RunStats = InitializeRunStats(P_EVO);          % initialise output, produces RunStats

tr = 1;
sl = 1;
gn = 1;

global PARAMS STATE CYCLES
ImportTumorState(sl,tr,P_EVO,POP);

global FINISH PARAMS PrC PrP STATE

FINISH = 0;
st = 0;
tumor_name = sprintf('out-%s-tr%.0f-st%.0f-%s.json',P_EVO.tum_lib_s, tr, st);
save_workspace(sprintf('out-%s-tr%.0f-st%.0f-0a-%s.json',P_EVO.tum_lib_s, tr, st,  'initial'));
ReplenishSubstrate2
save_workspace(sprintf('out-%s-tr%.0f-st%.0f-0b-%s.json',P_EVO.tum_lib_s, tr, st,  'ReplenishSubstrate'));

for st = 1:10
   Diffusion2
   save_workspace(sprintf('out-%s-tr%.0f-st%.0f-1-%s.json',P_EVO.tum_lib_s, tr, st,  'Diffusion'));
   IrradiateTumor_evo(st,tau)
   save_workspace(sprintf('out-%s-tr%.0f-st%.0f-2-%s.json',P_EVO.tum_lib_s, tr, st,  'IrradiateTumor'));
   SetLocalStates
   save_workspace(sprintf('out-%s-tr%.0f-st%.0f-3-%s.json',P_EVO.tum_lib_s, tr, st,  'SetLocalStates'));
   MetaboliseNutrients
   save_workspace(sprintf('out-%s-tr%.0f-st%.0f-4-%s.json',P_EVO.tum_lib_s, tr, st,  'MetaboliseNutrients'));
   SetGlobalStates
   save_workspace(sprintf('out-%s-tr%.0f-st%.0f-5-%s.json',P_EVO.tum_lib_s, tr, st,  'SetGlobalStates'));
   RepairCells
   save_workspace(sprintf('out-%s-tr%.0f-st%.0f-6-%s.json',P_EVO.tum_lib_s, tr, st,  'RepairCells'));
   CellDivision2
   save_workspace(sprintf('out-%s-tr%.0f-st%.0f-7-%s.json',P_EVO.tum_lib_s, tr, st,  'CellDivision'));
   RunStats = GetStats2(st, P_EVO, RunStats);    % update RunStats
   save_workspace(sprintf('out-%s-tr%.0f-st%.0f-8-%s.json',P_EVO.tum_lib_s, tr, st,  'RunStats'));
   FinishCheck
   save_workspace(sprintf('out-%s-tr%.0f-st%.0f-9-%s.json',P_EVO.tum_lib_s, tr, st,  'FinishCheck'));
   if FINISH, return; end
end

quit
