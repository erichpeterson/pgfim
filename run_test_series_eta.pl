#!/usr/bin/perl -w

#  print $dataset_sizes{'chess'};
if ($#ARGV < 1) {
	print "usage: program_name dataset_prefix taxonomy1 [taxonom2, taxonomy3, ...]\n";
	exit;
}

my $stats_dir = "executionstats/";
my $input_dir = "datasets/";
my $itemset_output_dir = "executionstats/";
my $input_data_file = $ARGV[0];
my @input_tax_files;

for ($it = 1; $it <= $#ARGV; $it++) {
	push(@input_tax_files, $ARGV[$it]);
}

my $itemset_output_file = "";
 
my %dataset_sizes = (
 	'chess' => 3196,
	'accidents' => 340183,
 	'connect' => 67557,
 	'mushroom' => 8124,
 	'T10I4D100K' => 100000,
 	);
 
my $TAU = 0.9;
my @ETA = ('0.5','0.6','0.7','0.8','0.9');
my $SUPPORT = 0.0;
my $str = "# " . " TAU_". $TAU . "DATA_" . $ARGV[0] . "\n";

my $exec_stats_fname = "TAU_" . $TAU . "DATA_" . $ARGV[0] . ".data";
open TIMEFILE, "+>>", "$stats_dir" . "$exec_stats_fname" or die $!;
 
$str = $str . "ETA\t";
foreach $tax (@input_tax_files) {
    $str = $str . $tax . "\t";
}
   
$str = $str . "\n";
my $cmd = "";
  
print TIMEFILE $str;

foreach $eta_item (@ETA) {
	print TIMEFILE "$eta_item        ";
 
    foreach $tax (@input_tax_files) {
    	$SUPPORT = $dataset_sizes{$input_data_file} * $eta_item;
    	#printf("$SUPPORT\n");
    	$itemset_output_file = "TAU_". $TAU . "ETA_" . $eta_item . "DATA_" . $input_data_file;
    	$itemset_output_file = $itemset_output_file . "TAX_" . $tax . ".out";
  		$cmd = "./PFGIM -inputDB $input_dir$input_data_file.dat -tau $TAU -minsup $SUPPORT -print 1 ";
  		$cmd = $cmd . "-inputXML $input_dir$tax "; 
    	$cmd = $cmd . "-exec $stats_dir$exec_stats_fname > $stats_dir$itemset_output_file";
  		printf("$cmd\n");
  		system($cmd);
  	}
  	print TIMEFILE "\n";  
}
close TIMEFILE;