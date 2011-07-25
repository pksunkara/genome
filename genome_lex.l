%{
	#include "y.tab.h"
	
	extern void count(void);
	extern void yyerror(char *);
%}

%%
\n { count(); }
[ \t\r\f]  { count(); }
"{"[^{}]*"}" { count(); }

"AAA" { count(); return AAA; }
"AAT" { count(); return AAT; }
"AAC" { count(); return AAC; }
"AAG" { count(); return AAG; }
"ATA" { count(); return ATA; }
"ATT" { count(); return ATT; }
"ATC" { count(); return ATC; }
"ATG" { count(); return ATG; }
"ACA" { count(); return ACA; }
"ACT" { count(); return ACT; }
"ACC" { count(); return ACC; }
"ACG" { count(); return ACG; }
"AGA" { count(); return AGA; }
"AGT" { count(); return AGT; }
"AGC" { count(); return AGC; }
"AGG" { count(); return AGG; }
"TAA" { count(); return TAA; }
"TAT" { count(); return TAT; }
"TAC" { count(); return TAC; }
"TAG" { count(); return TAG; }
"TTA" { count(); return TTA; }
"TTT" { count(); return TTT; }
"TTC" { count(); return TTC; }
"TTG" { count(); return TTG; }
"TCA" { count(); return TCA; }
"TCT" { count(); return TCT; }
"TCC" { count(); return TCC; }
"TCG" { count(); return TCG; }
"TGA" { count(); return TGA; }
"TGT" { count(); return TGT; }
"TGC" { count(); return TGC; }
"TGG" { count(); return TGG; }
"CAA" { count(); return CAA; }
"CAT" { count(); return CAT; }
"CAC" { count(); return CAC; }
"CAG" { count(); return CAG; }
"CTA" { count(); return CTA; }
"CTT" { count(); return CTT; }
"CTC" { count(); return CTC; }
"CTG" { count(); return CTG; }
"CCA" { count(); return CCA; }
"CCT" { count(); return CCT; }
"CCC" { count(); return CCC; }
"CCG" { count(); return CCG; }
"CGA" { count(); return CGA; }
"CGT" { count(); return CGT; }
"CGC" { count(); return CGC; }
"CGG" { count(); return CGG; }
"GAA" { count(); return GAA; }
"GAT" { count(); return GAT; }
"GAC" { count(); return GAC; }
"GAG" { count(); return GAG; }
"GTA" { count(); return GTA; }
"GTT" { count(); return GTT; }
"GTC" { count(); return GTC; }
"GTG" { count(); return GTG; }
"GCA" { count(); return GCA; }
"GCT" { count(); return GCT; }
"GCC" { count(); return GCC; }
"GCG" { count(); return GCG; }
"GGA" { count(); return GGA; }
"GGT" { count(); return GGT; }
"GGC" { count(); return GGC; }
"GGG" { count(); return GGG; }

. { yyerror("Illegal token"); }
%%
