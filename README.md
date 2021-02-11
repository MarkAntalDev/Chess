# Chess

Projekt: Sakk mestersége intelligencia c++ programnyelven.
Cél: BSc Diplomamunka
Fejleszti: Antal Márk Medárd
Felügyelő, Konzulens: Csáji Balázs Csanád Ph.D.

Fordítás futtatás:
compile_and_run.bat   /// fordít és futtat 
run.bat		      /// csak futtat

Működési elv, alapfogalmak:
A pályát egy 12*10 mezőből álló byte tömb reprezentálja.
A tömb 1 dimenziós, ami segíti az előre beégetett konstansok segítségével való bábu navigálást
A plusz mezők a pálya szélén lehetővé teszik a optimális időben történi bábu léptetést 
           (gyorsabban ellenőrizhető ha egy bábu kilép a pályáról)
