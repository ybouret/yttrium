use Algorithm::Networksort;
  

my $algorithm = "bosenelson";

for my $inputs (2..4) 
{
    #   
    # Generate the sorting network (a list of comparators).
    #
    # nwsrt() is a convenient short-hand
    # for Algorithm::Networksort->new().
    #
    my $nw = nwsrt(inputs => $inputs, algorithm => $algorithm);
    
    #
    # Print the title, the comparator list using the default
    # format, and a text-based Knuth diagram.
    #
    $nw->index_base([1 .. $inputs]);
    print $nw->title(), "\n";
     
    $nw->formats([ "%d,%d," ]);
    my $string = '{' . $nw->formatted();
    substr($string, -1, 1) = '}';    # Overwrite the trailing comma.
    print $string, ";\n";
   
    #print $nw->formatted(),"\n";

    my $filename = "nw" . $inputs . ".hxx";
    print $filename,  ";\n";
    open(FH,'>',$filename);
    print FH "const unsigned nwTable$inputs\[\]=", $string, ";\n";
    print FH "const unsigned nwCount$inputs = sizeof(nwTable$inputs)/sizeof(nwTable$inputs\[0\])/2;\n";
    close(FH)
}
 
