echo "Removing first rootfile"
rm results/output_*.root
# sample
for i in 0 1; do
	echo "sample choice = $i"
	#cem
	for j in 0 1 2; do
		echo "mode = $j"
		root.exe -b -l -q minimal.C\($i\,$j\,0\)
	done
done

echo "Finished"

