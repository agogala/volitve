#!/usr/bin/perl
# $ProjectId$
#
# $Id: mkpie.pl 1.1 Fri, 21 Nov 1997 17:06:57 +0000 andrej $
#
# Nari¹i pie graph vseh kandidatov za dani dan

use GIFgraph::pie;
use Pg;
#use getopt;

# Izraèunaj pie graph:

$conn = Pg::connectdb("dbname = volitve");
Pg::doQuery($conn, "select papir_id, tecaj from tecajnica where datum='yesterday'", \@db_data);

$sum = 0;

for $i ( 0 .. $#db_data ) {
    $sum += $db_data[$i][1];
}

@data = ([],[]);

for $i ( 0 .. $#db_data ) {
    push(@{@data[0]}, sprintf("%s: %5.2f", unpack("A8",pack("a8",$db_data[$i][0])), ($db_data[$i][1]/$sum) * 100));
    push(@{@data[1]}, $db_data[$i][1]);

}

$big_graph = new GIFgraph::pie( );


$big_graph->set( 'title' => 'Porazdelitev poravnalnih cen',
#                 'label' => 'Datum',
                 'axislabelclr' => 'black' );

$big_graph->plot_to_gif( "/opt/borza/HTML/stat/napoved.gif", \@data );

$convertor = "/net/rozle/export/share/sol/bin/convert"; 

system("$convertor -geometry 100x100 -colors 16 /opt/borza/HTML/stat/napoved.gif /opt/borza/HTML/stat/napoved-chip.gif");

exit;

 
