## Pretvorba windows-1250 v iso-8859-2, samo ¹umniki...
## Prekopirano iz iso8859.pl
##---------------------------------------------------------------------------##
##  File:
##	@(#)  iso8859.pl 1.2 97/06/03 @(#)
##  Author:
##      Earl Hood       ehood@medusa.acs.uci.edu
##  Description:
##      Routines to process data encoded in iso8859 character sets.
##---------------------------------------------------------------------------##
##    Copyright (C) 1996        Earl Hood, ehood@medusa.acs.uci.edu
##
##    This program is free software; you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation; either version 2 of the License, or
##    (at your option) any later version.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with this program; if not, write to the Free Software
##    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
##    02111-1307, USA
##---------------------------------------------------------------------------##

package win_cs;

##---------------------------------------------------------------------------
##	ISO-8859-2: Latin-2
##---------------------------------------------------------------------------

%WINDOWS1250 = (
  #--------------------------------------------------------------------------
  # Hex Code    Result	        # Unicode code and description
  #--------------------------------------------------------------------------
    0x8C,       0xA9,           # 0160 LATIN CAPITAL LETTER S WITH CARON
    0x8E,       0xAE,           # 017D LATIN CAPITAL LETTER Z WITH CARON
    0x9A,       0xB9,           # 0161 LATIN SMALL LETTER S WITH CARON 
    0x9E,       0xBE,           # 017E LATIN SMALL LETTER Z WITH CARON
    0xC8,       0xC8,           # 010C LATIN CAPITAL LETTER C WITH CARON 
    0xE8,       0xE8,           # 010D LATIN SMALL LETTER C WITH CARON
);

###############################################################################
##	Routines
###############################################################################

sub win2lat2 {
    local($data, $charset, $only8bit) = ($_[0], $_[1], $_[2]);
    local($ret, $offset, $len) = ('', 0, 0);

    # Get mapping (this method works for Perl 4 and 5)
    $charset =~ tr/a-z/A-Z/;
    $charset =~ tr/-/_/;
    local(*map) = ("${charset}");

    # Convert string
    $len = length($data);
    while ($offset < $len) {
	$char = unpack("C", substr($data, $offset++, 1));
	$ret .= ($map{$char} || pack("C", $char));
    }
    $ret;
}

##---------------------------------------------------------------------------##
1;
