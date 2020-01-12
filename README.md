# Finding mutations using 3th generation sequencing
This is a project for [Bioinformatics class](https://www.fer.unizg.hr/predmet/bio). It's about finding mutations using 3th gen sequencing method.

## Test data
https://www.dropbox.com/s/iqi3nbf839wrshh/Bioinfo_19_20_train_data.tar.gz?dl=0

**Description**

    ecoli.fasta - reference genome
    ecoli_mutated.csv - list of mutations to find
    ecoli_mutated.report - reading statistics and reference implementation's Jaccards score
    ecoli_simulated_reads.fasta - readings obtained by sequencing the mutated genome
    jaccard.py - script for computing Jaccard's score

    Same goes for another dataset lambda.

## References

[1] Heng Li. [Minimap and miniasm: fast mapping and de novo assembly for noisy long sequences](https://doi.org/10.1093/bioinformatics/btw152). Bioinformatics, 32(14):2103–2110, 03 2016.

[2] Michael Roberts, Wayne Hayes, Brian R. Hunt, Stephen M. Mount, and James A. Yorke. [Reducing storage requirements for biological sequence comparison](https://doi.org/10.1093/bioinformatics/bth408). Bioinformatics, 20(18):3363–3369, 07 2004.