for folder in blackbox/*
do
  for dataset in "$folder"/*[^.dat]
  do
    rm -f out.txt
    for file in "$dataset"/*
    do
      inst=${file#"$dataset/"}
      inst=${inst%".mwcnf"}
      inst=${inst/"wuf"/"uf"}
      printf "$inst " >> out.txt
      java -jar sawsat3.jar < "$file" 1>>out.txt 2>/dev/null
    done
    sort out.txt > "$dataset-prg.dat"
    df=$(diff -y --suppress-common-lines "$dataset-prg.dat" "$dataset-opt.dat" | wc -l)
    echo "Different: $df / 100"
  done
done
