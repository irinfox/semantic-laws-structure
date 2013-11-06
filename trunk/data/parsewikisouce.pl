#! /usr/bin/perl

use strict;
use Encode;
use utf8;
use XML::Parser;
use File::Path;

my $start = time();

my $dump = shift @ARGV;

binmode(STDOUT, ':encoding(utf8)');
binmode(STDERR, ':encoding(utf8)');
binmode($dump, ':encoding(utf8)');

my $page;
my $title;
my $tag_text;
my $title_name;
my $text = undef;
my $i = 0; #for file names

my $struct = "codes_structure.txt"; #file for keeping structure
open (STRUCT,">$struct") or die "cannot create $!"; 
binmode(STRUCT, ':encoding(utf8)');
print STRUCT "#code;redaction;some_parts{0,4};chapter;article\t\tfilename\n";

#vars for statutory acts
my $codes = 'кодекс ([а-я ]+?)?РФ(\/Редакция \d{2}\.\d{2}\.\d{4})?\/(Глава|Раздел) ';

my $kate = '(ГОСТ|Указ Президента РФ|Федеральный закон)';

my $dir = 'res';
if ( -d "res"){
 rmtree($dir);
}
mkdir $dir;

#parse to extract articles
my $parser = XML::Parser->new(Handlers=>{Start => \&tag_start, Char => \&tag_text,  End => \&tag_end});
$parser->parsefile($dump);

#while (my ($key, $value) = each %names){
#     print NAMES "$key\t$value\n";
#}

print STDERR time()-$start."\n";


#------------subs------------------------------------------
###########################################################

sub tag_start{
  my ($expat, $tag_name, %attr) = @_;
 
  if ($tag_name eq 'page'){
        $page = $tag_name;
  }
 
  if ($tag_name eq 'title'){
        $title = $tag_name;
     }
  if ($tag_name eq 'text'){
        $tag_text = $tag_name; 
#	print "$tag_text tag\n";
  }
   
}


sub tag_text{
   my ($expat, $string) = @_;
   if ($title eq 'title'){
       $title_name = $string;
      # print "in title tag: ".$string."\n";
   }
   
   if ($tag_text eq 'text' && $title_name =~ /$codes/i){ 
  # if ($tag_text eq 'text' && $title_name =~ /$kate/i){ 
      $text = $text.$string;   
     # print $string;
   }
}  

sub tag_end{
  my ($expat, $tag_name) = @_;
 
  if ($tag_name eq 'title'){
     $title = undef;
  }

  if ($tag_name eq 'page'){
     $page = undef;
  }
 
  if ($tag_name eq 'text'){
     if ($text) {
	parse_text($text);
   #     $i++;      
   #     my $f = $dir.'/doc'.$i.".txt"; 

    #    open (FILE,">$f") or die "cannot create $!"; 
    #    binmode(FILE, ':encoding(utf8)');
    #    print FILE $text;
 #	close FILE;
     }
     $tag_text = undef;
     $text = undef;
  } 
  
}

sub parse_text {

  my $t = shift;
  #open (STR, "<$t") or die "$!";
  my $start = 'false';
  my $f; 
    
  my ($code, $red, $chapter) = split/\//, $title_name; 
  if (!$chapter){
     $chapter = $red;
     $red = "";
  }  
  
  my $parts; 
 
  for(split /^/,$t){
        
    if ($_ =~ /^\<center\>(.+)\<\/center\>/){
        my $tmp = $1;
	$tmp =~ s/\'//g;
        $tmp =~ s/\&lt\;.+?\&gt\;//;
        if ($parts){ 
            $parts = $parts.';'.$tmp;
        } else { $parts = $tmp; } 
    }
    
    if ($_ =~ /^(\s+)?\={3,4}\s?Статья \d+/){
        $start = 'true';

        $i++;      
        $f = $dir.'/article'.$i.".txt"; 

        open (FILE,">$f") or die "cannot create $!"; 
        binmode(FILE, ':encoding(utf8)');
        my $art_name = remove_markup($_);
        print FILE $art_name; 
        print FILE "\n\n";

	print STRUCT "$code;$red;$parts;$chapter;$art_name\t\t$f\n";
	 
        next; 
    }
    if ($_ =~ /^(\s+)?\={2,3}\s?Примечания/){
       $start = 'false';
    }
        
    if ($start eq 'true'){
        my $s = remove_markup($_);
        print FILE $s; 
     }
  }
  close STR;
}

sub remove_markup{
    my $tmp = shift;
    
    $tmp =~ s/(\s+)?={3,4}(\s+)?//g;
    $tmp =~ s/\[\[.+?\|//g;
    $tmp =~ s/\]\]//g;
    $tmp =~ s/\<ref\>.+?\<\/ref\>//g;

    return $tmp; 
}
