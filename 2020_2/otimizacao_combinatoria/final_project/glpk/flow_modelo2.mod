set VERTICES;
set LABELS;
set ARCS within (VERTICES cross VERTICES);
set ARC_LABELS within (ARCS cross LABELS);

param in_flow{ARCS};
param out_flow{ARCS};

var x{(i,j) in ARCS} >= 0, <= 1;
var l{(i, j, k) in ARC_LABELS} >= 0, <= 1;
var l2{i in LABELS};
var flow{(i, j) in ARCS};

minimize cost: sum{i in LABELS} l2[i];

s.t. ONE_ARC{(i,j) in in_flow} sum{x[i,j]} = 1;

s.t. BALANCE{i in VERTICES}:
          sum{j in VERTICES: (i,j) in in_flow} x[i,j] 
        - sum{j in VERTICES: (j,i) in out_flow} x[j,i]
        = 1;
        
s.t. FIJ_SEQ{(i,j) in ARCS}:
          flow[i,j] <= (n-1)x[i,j];

s.t. FIJ_GEQ{i in ARCS}:
          flow[i,j] => (n-1)x[i,j];

s.t. SUM_MIN{(i,j,k) in l2}:
          /*coisa 4 lá do treco*/

s.t. LIMIT_FLOW{(i,j) in ARCS}: flow[i,j] => 0;

end;
