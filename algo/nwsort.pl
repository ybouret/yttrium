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
    print $nw->formatted(),"\n";
}
 
