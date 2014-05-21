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
print STRUCT "#code;redaction;some_parts{0,4};chapter;subparts{0,2);article\t\tfilename\n";

my %codedir = ( 
   'Арбитражный процессуальный кодекс РФ' => 'apk',
   'Бюджетный кодекс РФ' => 'bk',
   'Гражданский кодекс РФ' => 'gk',
   'Гражданский процессуальный кодекс РФ' => 'gpk',
   'Жилищный кодекс РФ' => 'jk',
   'Земельный кодекс РФ' => 'zk',
   'Кодекс внутреннего водного транспорта РФ' => 'wtk',
   'Кодекс РФ об административных правонарушениях' => 'koap',
   'Кодекс торгового мореплавания РФ' => 'swk',
   'Налоговый кодекс РФ' => 'nk',
   'Семейный кодекс РФ' => 'sk',
   'Таможенный кодекс РФ' => 'old',
   'Трудовой кодекс РФ' => 'tk',
   'Уголовно-исполнительный кодекс РФ' => 'uik',
   'Уголовно-процессуальный кодекс РФ' => 'upk',
   'Уголовный кодекс Российской Федерации' => 'uk'
);

my $codes = 'кодекс ([а-я ]+?)?(РФ|Российской Федерации)( об [а-я ]+?)?(\/Редакция \d{1,2}\.\d{1,2}\.\d{4})?\/(Глава|Раздел|(,часть)) ';

my $kate = '(ГОСТ|Указ Президента РФ|Федеральный закон)';

my $dir = 'res';
if ( -d "res"){
 rmtree($dir);
}
mkdir $dir;

for my $subdir (values %codedir){
  if ( -d $subdir){
    rmtree($subdir);
  }
  mkdir $dir."/".$subdir;
}

if ( -d "apk_old"){
 rmtree("apk_old");
}
mkdir "apk_old";

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
       if ($title_name =~ /(^Страница:)/){
           $title_name =~ s/$1//;
       }
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
  my $subpart;

  for(split /^/,$t){
    
    if ($_ =~ /^\<center\>(.+)\<\/center\>/){
        my $tmp = $1;
	$tmp =~ s/\'//g;
	$tmp =~ s/\<\/?small\>//g;
        $tmp =~ s/\&lt\;.+?\&gt\;//;
        
        if ($tmp =~ /\<\/?big\>/){
	    $tmp =~ s/\<\/?big\>//g;
            $subpart = $subpart.';'.$tmp;
            next;
        }
        
        if ($parts){ 
            $parts = $parts.';'.$tmp;
        } else { $parts = $tmp;}
        next; 
    }
    if ($_ =~ /\={3,4}\s?(§ \d+\..+)\s?\={3,4}/){
       $subpart = $1;
    } 
    if ($_ =~ /^(\s+)?\={3,4}\s?Статья \d+/){
        $start = 'true';

        $i++;
        if (exists $codedir{$code}){      
           if ($codedir{$code} eq "apk" && $red !~ /2010/) {
	         $f = 'apk_old/article'.$i.".txt";
              }
           else {
	     $f = $dir.'/'.$codedir{$code}.'/article'.$i.".txt";
           }      
        } else {$f = $dir.'/article'.$i.".txt";}

        open (FILE,">$f") or die "cannot create $!"; 
        binmode(FILE, ':encoding(utf8)');
        my $art_name = remove_markup($_);
        print FILE $art_name; 
        print FILE "\n\n";

	print STRUCT "$code;$red;$parts;$chapter;$subpart;$art_name\t\t$f\n";
	 
        next; 
    }
    if ($_ =~ /^(\s+)?\={2,3}\s?Примечания/){
       $start = 'false';
    }
        
    if ($start eq 'true'){
        my $s = remove_markup($_);
        if ($s =~ /^\(в ред\./){
           next;
        }
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
    $tmp =~ s/\'{2,3}//g;

    return $tmp; 
}
