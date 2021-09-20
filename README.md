# How to compile
  1. Go into "build" directory.
  2. Enter below on the terminal.
    ``` cmake .. ```
  3. Enter below on the terminal.
    ``` make all ```
  4. Then, executable file "tsp" appeared in "build" directory

# How to run
  ## (臨時)  
  まだRandomTourと2optしか実装されていないため
  必ず！以下のようにターミナルに入力して実行すること

    ``` 
      ./tsp TSPNAME RT 2OPT FLOAT SEED
    ```
  TSPNAME : /data/  TSP_VLSI_DATASET/ ディレクトリーにあるファイル名  
  RT      : そのまま "RT" を入力  
  2OPT    : そのまま "2OPT" を入力  
  FLOAT   : 適当な実数を入力  
  SEED    : 適当な自然数を入力  

  例

    ```
      ./tsp pma343.tsp RT 2OPT 12.1 2021
    ```