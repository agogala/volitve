#!/usr/bin/perl
# $ProjectId$
#
# $Id: mkgraph.pl 1.1 Fri, 21 Nov 1997 17:06:57 +0000 andrej $
#
# Nari¹i grafe:
# pie chart porazdelitev cen
# gibanje poravnalnih cen do sedaj
# gibanje porazdelitev poravnalnih cen do sedaj

use GIFgraph::pie;
use GIFgraph::lines;
use Pg;
#use getopt;

# nastavitve:
$convertor = "/net/rozle/export/share/sol/bin/convert"; 
#$destdir = "/opt/borza/HTML/stat/";
$destdir = "./";
#$dbname = "volitve";

$conn = Pg::connectdb("dbname = volitve");

# Popravi datum:
sub fixdate {
    return substr($_[0], 3, 2) . "." . substr($_[0], 0, 2);
}

# Izraèunaj pie graph:
sub Pie {
Pg::doQuery($conn, "select papir_id, tecaj from tecajnica where datum='yesterday'", \@db_data);
  
  $sum = 0;

  for $i ( 0 .. $#db_data ) {
      $sum += $db_data[$i][1];
  }

  @data = ([],[]);

  for $i ( 0 .. $#db_data ) {
      push(@{@data[0]}, sprintf("%s: %5.2f", unpack("A8",pack("a8",$db_data[$i][0])), ($db_data[$i][1]/$sum) * 100));
      push(@{@data[1]}, ($db_data[$i][1]/$sum)*100);
  }

  $pie_graph = new GIFgraph::pie( );
  $pie_graph->set( 'title' => 'Porazdelitev poravnalnih cen',
#                 'label' => 'Datum',
		  'axislabelclr' => 'black' );
#  print "About to plot\n";
  $pie_graph->plot_to_gif( "${destdir}napoved.gif", \@data );
  system("$convertor -geometry 100x100 -colors 16 ${destdir}napoved.gif ${destdir}napoved-chip.gif");
}

sub Line {
# Nari¹i line grafe:

  Pg::doQuery($conn, "select datum, sum(tecaj) from tecajnica where datum<'today' group by datum order by datum", \@db_sum);

  Pg::doQuery($conn, "select papir_id from papirji order by papir_id", \@db_papirji);

    @cene_data = ([]);
    @przd_data = ([]);
    @sum_data = ([],[]);
    %papirji = ();

    for $i ( 0 .. $#db_papirji ) {
 	push(@cene_data, []);
 	push(@przd_data, []);
	$papirji{$db_papirji[$i][0]} = $i+1;
    }

    for $i ( 0 .. $#db_sum) {
 	$datum = &fixdate($db_sum[$i][0]);
	$db_datum = $db_sum[$i][0];
	$sum = $db_sum[$i][1];
 	push(@{@cene_data[0]}, $datum);
 	push(@{@przd_data[0]}, $datum);
	push(@{@sum_data[0]}, $datum);
	push(@{@sum_data[1]}, $sum);
      Pg::doQuery($conn, "select papir_id, tecaj from tecajnica where datum='$db_datum'", \@db_data);	
 	for $j ( 0 .. $#{@db_papirji}) {
	    push(@{@cene_data[$j+1]}, 0);
	    push(@{@przd_data[$j+1]}, 0);
	}
	for $j ( 0 .. $#{@db_data}) {
	    $cene_data[$papirji{$db_data[$j][0]}][$i] = $db_data[$j][1];
	    $przd_data[$papirji{$db_data[$j][0]}][$i] = $db_data[$j][1]/$sum;
	}
    }
    
    $cene_graph = new GIFgraph::lines(600, 400);
    $cene_graph->set( 'title' => 'Poravnalne cene',
#                 'label' => 'Datum',
		     'axislabelclr' => 'black',
		     'line_width' => 10);
    $cene_graph->plot_to_gif( "${destdir}cene.gif", \@cene_data );
    system("$convertor -geometry 100x100 -colors 16 ${destdir}cene.gif ${destdir}cene-chip.gif");

    $przd_graph = new GIFgraph::lines(600, 400);
    $przd_graph->set( 'title' => 'Porazdelitev poravnalnih cen',
		     'axislabelclr' => 'black',
		     'line_width' => 10);
    $przd_graph->plot_to_gif( "${destdir}porazd.gif", \@przd_data );
    system("$convertor -geometry 100x100 -colors 16 ${destdir}porazd.gif ${destdir}porazd-chip.gif");

    $vsota_graph = new GIFgraph::lines(600, 400);
    $vsota_graph->set( 'title' => 'Vsota poravnalnih cen',
		     'axislabelclr' => 'black',
		     'line_width' => 10 );
    $vsota_graph->plot_to_gif( "${destdir}vsota.gif", \@sum_data );
    system("$convertor -geometry 100x100 -colors 16 ${destdir}vsota.gif ${destdir}vsota-chip.gif");

}

Pie;
#Line;

exit;

 
