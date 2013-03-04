#!/bin/sh

##-----------------------------------
## help
##-----------------------------------
# IN : $1 est un code d'erreur
# OUT : Rien
# Termine le programme car les paramètres passés sont incorrects
# RETOUR : Rien
function help
{
    case $1 in
	1)
	    echo "ERREUR: Il n'y a pas assez de paramètres"
	    ;;
	2)
	    echo "ERREUR: $in n'est pas un fichier"
	    ;;
    esac
    echo "tableur   [-in feuille]   [-out  résultat]   [-scin sep]   [-scout  sep]   [-slin sep]   [-slout  sep]   [-inverse Les différentes options sont les suivantes, leur ordre d'apparition n'est pas et ne doit pas être fixé."
    echo " -in feuille : permet d'indiquer dans quel fichier (feuille) se trouve la feuille de calculs. Si cette option n'est pas spécifiée sur la ligne de commande, la feuille de calculs sera lue depuis l'entrée standard."
    echo "- -out résultat : permet d'indiquer dans quel fichier (résultat) doit être écrite la feuille calculée. Si cette option n'est pas spécifiée, le résultat sera affiché sur la sortie standard."
    echo "- -scin sep : permet de spécifier le séparateur (sep) de colonnes de la feuille de calculs initiale. Par défaut si cette option n'est pas spécifiée, ce séparateur est la tabulation."
    echo "- -slin sep : permet de spécifier le séparateur (sep) de lignes de la feuille de calculs initiale. Par défaut si cette option n'est pas spécifiée, ce séparateur est le retour chariot."
    echo "- -scout sep : permet de spécifier le séparateur (sep) de colonnes de la feuille calculée. Par défaut si cette option n'est pas spécifiée, ce séparateur est identique au séparateur de colonnes de la feuille de calculs initiale ;"
    echo "- -scout sep : permet de spécifier le séparateur (sep) de lignes de la feuille calculée. Par défaut si cette option n'est pas spécifiée, ce séparateur est identique au séparateur de lignes de la feuille de calculs initiale."
    echo "- -inverse : cette option provoque l'inversion lignes/colonnes de la feuille calculée."
    exit $1
}


##-----------------------------------
## ij
##-----------------------------------
# IN : $1 est de la forme licj où i et j st des nombres
# OUT : rien
# Retour : $i et $j
function ij
{
    local i
    local j
    i=`echo $1 | cut -d c -f1 | cut -d l -f2`
    j=`echo $1 | cut -d c -f2 `
    echo $i $j
}


##-----------------------------------
## cel
##-----------------------------------
# IN : $i et $j
# OUT : Affiche la cellule correspondant à $i$j
# Retour : rien
function cel
{
    local i
    local j
    local saveIFS
    local sortie
    i=$1
    j=$2
    saveIFS=$IFS
    IFS=$slin
    set $fichier

    # Extraction de la ligne
    if [ $# -ge $i ]  # Nb de lignes OK
	then
	shift `expr $i - 1`
	IFS=$scin
	set $1

	# Extraction de la colone
	if [ $# -ge $j ] # Nb de colones OK
	    then
	    shift `expr $j - 1`
	    sortie=$1

	else  # Nb de colones KO
	    sortie="#ERREUR-Colone#"
	fi

    else # Nb de lignes KO
	sortie="#ERREUR-ligne#"
    fi

    IFS=$saveIFS
    echo $sortie
}

##-----------------------------------
## somme
##-----------------------------------
# IN : Une liste de valeurs
# OUT : La somme de toute la liste des valeurs passées en param
# S'il y a un probleme de calcul, #RTFM# est repéré par les fonctions utilisant somme
# Retour :
function somme
{
    local res
    res=0
    while test $# -ge 1
      do
      if echo "$1" | grep -q -E "#RTFM#"
	  then
	  #s'il y a une erreur de calcul 'somme' revoi "#RTFM# xxxx" 
	  # détectable par d'autres fontions
	  res="#RTFM# valeur non numérique pour faire une somme"
	  shift $#
      else
      	  res=`echo "$res + $1" | bc -l`
      	  shift
      fi
    done
    echo "$res"
}



##-----------------------------------
## moyenne
##-----------------------------------
# IN : liste de valeurs
# OUT : 
# Retour : la moyenne de la liste
function moyenne
{
    # On doit avoir une sélection non nulle (division par zéro!!)
    if [ $# -eq 0 ]
	then
	echo "#RTFM# : la sélection est nulle"
    # On ne doit pas avoir d'erreurs non plus
    elif echo "$*" | grep -q -E "#RTFM#"
	then
	echo "#RTFM# valeur non numérique pour faire une moyenne"
    else
	echo "`somme $*` / $#" | bc -l
    fi
}


##-----------------------------------
## variance
##-----------------------------------
# IN : liste de valeurs 
# OUT : 
# Retour : la variance des valeurs
function variance
{
    local res
    local moy
    local a
    local all
    
    all=$#
    # On doit avoir une sélection non nulle 
    if [ $# -eq 0 ]
	then
	echo "RTFM# : la sélection est nulle"
      # On ne doit pas avoir d'erreurs non plus
    elif echo "$*" | grep -q -E "#RTFM#"
	then
	echo "#RTFM# valeur non numérique pour faire une variance"
    else
	moy=`moyenne $*` 
	res=0
	while [ $# -ge 1 ]
	  do
	  #pas de gestion d'erreur dans le calcul des cases :
	  #impossible car deja testé au dessus et (encore) dans 'moyenne'
	  a=`echo "$1 - $moy" | bc -l `
	  a=`echo "$a * $a" | bc -l `
	  res=`echo "$res + $a" | bc -l `
	  shift
	done
	echo "$res / $all" | bc -l
    fi
}


##-----------------------------------
## VirguleSiOp
##-----------------------------------
# IN : chaine de caracteres (opérateurs ou non)
# OUT :
# Retour : Nombre de virgules appartenant aux opérateurs
function VirguleSiOp
{
    local operateur
    local tailleop
    local interieur
    local bool
    local op

    if echo "$1" | grep -q -E "\("
	then
        #l'élément avant la parenthese
	operateur=`echo "$1" | cut -d "(" -f1`
        #taille de l'op avant la parenthese
	tailleop=`echo "$operateur"| wc -c | sed "s/ //g"`
        #l'élément apres la parenthese
	interieur=`echo "$1" | cut -c"$[$tailleop + 1]"-`
	
#s'il y a une parenthese ouvrante on teste si avant il y a un op et on ajoute les virgules qu'il a entre ses elements(0,1,2).
	
	bool=0
	  #si c un operateur a 2 virgule
	if test "$operateur" = "substitute"
	    then 
	    echo $[2 + `VirguleSiOp $interieur`]
	    
	else
          #si c un operateur a 1 virgule
	    for op in  "\+ - \* / \^ somme moyenne variance ecarttype mediane min max concat display"
	      do
	      if echo "$operateur" | grep -q -E "$op"      
		  then
		  echo $[1 + `VirguleSiOp $interieur`]
		  bool=1
	      fi
	    done
	    #bollean dans le for au dessus et if(!bool), pour evité testé lop, sachant  quil é pas ok
	    if [ $bool -eq 0 ]
		then
		for op in 'ln e sqrt lenght size lines shell'
		  do
                  #si operateur sans virgul
		  if [ "$operateur" = "$op" ]
		      then
		      echo $[0 + `VirguleSiOp $interieur`]
		      bool=1
		  fi
		done
		
		if [ $bool -eq 0 ]
		#pas d'opérateur (chaine de caracteres quelconque)
		    then    
		    echo "0"
		fi
	    fi
	fi
    else
     #plus d'opérateur (chaine de caracteres)
	echo "0"
    fi
}




##-----------------------------------
## separation
##-----------------------------------
# IN : op(var1,var2) sans l'operateur  var1,var2
# OUT : 
# Sépare les 2 elements d'une opération (trouve la bonne virgule)
# Retour : $var1 $var2
function separation
{
 #l'operateur et ses parentheses sont deja enlevee par moteur on a: var1,var2
#car ici on ne distingue pas l'op en particulier
    local saveIFS
    local var
    local compteurvirgule
    local var1   
    local var2
    local souselem

    saveIFS=$IFS
    
    compteurvirgule=0;
    var1=""
    var2=""
    IFS=","
    var=$1

#Une chaine de caracteres pourra contenir des parentheses. Mais pas de virgules.
#ex:  concat((aa(a))(,()()
#Une chaine de caracteres pourra contenir des operateur, s'il ne sont pas en début e chaine.
#ex: concat(a+(aa+a,a))

    for souselem in $var
      do
      IFS=$saveIFS
      if test $compteurvirgule -gt -1
	  then 
	  compteurvirgule=$[ $compteurvirgule + `VirguleSiOp $souselem` ]
	  
          #tantque l'on est pas arrivé a la bonne virgule, on est dans $var1
	  if test -z "$var1"  # -z = if(chaine vide)
	      then
	      var1="$souselem"
	  else
	      var1="$var1,$souselem"
	  fi
      else
          #compteurvirgule=-1 ssi on a 'passé' les virgules des autres opérateurs
	  if test -z "$var2"
	      then
	      var2="$souselem"
	  else
	      var2="$var2,$souselem"
	  fi
      fi
      #on passe une virgule donc on décompte
      compteurvirgule=$[ $compteurvirgule - 1 ]
      IFS=","
    done
    IFS=$saveIFS
    
    echo "$var1 $var2"
}


##-----------------------------------
## is_real
##-----------------------------------
# IN : Une chaine de caractère à identifier comme un réél
# OUT : La regex identifiant un réél
# Retour :
function is_real
{
    echo $1 | grep -q -E "^([\+\-])?[0-9]+([.]{1}[0-9]+)?$"
}


##-----------------------------------
## is_real_ge_0
##-----------------------------------
# IN : Une chaine de caractère à identifier comme un réél >= 0
# OUT : La regex identifiant un réél >= 0 
# Retour :
function is_real_ge_0
{
    echo $1 | grep -q -E "^([\+])?[0-9]+([.]{1}[0-9]+)?$"
}


##-----------------------------------
## separeMoteur
##-----------------------------------
# IN : op(var1,var2)
# OUT :
# Enleve l'opérateur et ses parentheses et appelle 'separation' 
# Retour : var1 var2 (retour de 'séparation')
function separeMoteur
{
    local cel
    local nb
    local separ
    local c
    cel=$1
    nb=$[ $2 + 2 ]
    separ="`echo $cel | cut -c$nb-`"
    c=$[ `echo "$separ" | wc -c | sed "s/ //g"` - 2 ]
    separ=`echo "$separ" | head -c"$c"`
    echo `separation "$separ"`
}


##-----------------------------------
## separeSubstitute
##-----------------------------------
# IN : op(var1,var2,var3)
# OUT : 
# Enleve l'opérateur et ses parentheses et appelle 2 fois 'separation',
# dabord pour séparer 'var1' de 'var2,var3' et ensuite pour séparer 'var2' de 'var3'
# Retour : var1 var2 var3
function separeSubstitute
{
    local cel
    local nb
    local separ
    local c
    cel=$1
    nb=$[ $2 + 2 ]
    separ="`echo $cel | cut -c$nb-`"
    c=$[ `echo "$separ" | wc -c | sed "s/ //g"` - 2 ]
    separ=`echo "$separ" | head -c"$c"`
    
    separ=`separation "$separ"`
    set - $separ
    #envoi de 'var1', séparation de 'var2,var3' et envoi de 'var2 var3'
    echo $1 `separation $2`
}


##-----------------------------------
## moteur
##-----------------------------------
# IN : calcul a effectué (ou chaine de characteres a retourner)
# OUT :
# Ici arrive les cellules a calculer (=).
# Récusivement moteur va différencier les opérateurs
# séparer et calculé les différents éléments de ces opérateurs jusqu'à la 'chaine de caractére' (peut etre un nombre) qui est résultat.
# Retour : chaine de caractere | (nombre)
function moteur
{
    local cel
    local lacase
    local saveIFS
    local separ
    local min
    local max
    local lasomme
    local res
    local fre
    local cmde
    local milieu
    local reste
    local var1
    local var2
    local op
    
    cel=$1

    # cel
    if echo $cel | grep -q -E "^l[0-9]+c[0-9]+$"
	then
	saveIFS=$IFS
	IFS=" "
	lacase=`ij $cel`
	IFS=$saveIFS
	fre=`cel $lacase`
	analyse_debut "$fre"
	
    # + - *  (val1,val2)
    elif  echo $cel | grep -q -E "^[-\+\*]{1}\(.+,.+\)"
	then
	set - `separeMoteur $cel 1`
	var1=`moteur $1`
	var2=`moteur $2`
	op=`echo "$cel" | head -c 1`
	# $var1 doit etre un réél
	if ! is_real $var1
	    then
	    echo -n "#RTFM# : $var1 n'est pas un réél"
	# $var2 doit etre un réél
	elif ! is_real $var2
	    then
	    echo -n "#RTFM# : $var2 n'est pas un réél"
	else
	    echo "$var1 $op $var2" | bc -l
	fi

    # /(val1,val2)
    elif  echo $cel | grep -q -E "^[\/]\(.+,.+\)$"
	then
	set - `separeMoteur $cel 1`
	var1=`moteur $1`
	var2=`moteur $2`

	if ! is_real $var1
	    then
	    echo -n "#RTFM# : $var1 n'est pas un réél"
	# $var2 doit etre un réél
	elif ! is_real $var2
	    then
	    echo -n "#RTFM# : $var2 n'est pas un réél"
	# $var2 doit etre différent de zéro
	elif echo $var2 | grep -q -E "^([\+\-])?[0]+([.]{1}[0]+)?$"
	    then
	    echo -n "#RTFM# : division par zéro interdite"
	else
	    echo "$var1 / $var2" | bc -l
	fi

    # e(val1)
    elif echo $cel | grep -q -E "^e\(.+\)$"
	then
	set - `separeMoteur $cel 1`
	var1=`moteur $1`
	# e() n'accepte que des rééls
	if ! is_real_ge_0 $var1
	    then
	    echo "#RTFM# : $var1 n'est pas un réél"
	else
	    echo "e($var1)" | bc -l
	fi

    # ln(val1)
    elif echo $cel | grep -q -E "^ln\(.+\)$"
	then
	set - `separeMoteur $cel 2`
	var1=`moteur $1`
	# ln() n'accepte que des rééls supérieurs à zéro
	if ! is_real_ge_0 $var1
	    then
	    echo "#RTFM# : $var1 n'est pas un réél supérieur à zéro"
	elif echo $var1 | grep -q -E  "^([\+\-])?[0]+([.]{1}[0]+)?$"
	    then
	    echo "#RTFM# : $var1 n'est pas un réél supérieur à zéro"
	else
	    echo "l($var1)" | bc -l
	fi

    # sqrt(val1)
    elif echo $cel | grep -q -E "^sqrt\(.+\)$"
	then
	set - `separeMoteur $cel 4`
	var1=`moteur $1`
	# ln() n'accepte que des rééls supérieurs ou égal à zéro
	if ! is_real_ge_0 $var1
	    then
	    echo "#RTFM# : $var1 n'est pas un réél supérieur ou égal à zéro"
	else
	    echo "sqrt($var1)" | bc -l
	fi

    # somme(cel1,cel2)
    elif echo $cel | grep -q -E "^somme\(.+,.+\)$"
	then
	set - `separeMoteur $cel 5`
	set - `get_selection $1 $2`
	# On ne doit pas avoir d'erreurs dans le calcul des cases
	if echo "$*" | grep -q -E "#RTFM#"
	    then
	    echo "#RTFM# somme impossible : valeur(s) non numérique"
	else
	    echo `somme $*`
	fi
	
    # moyenne(cel1,cel2)
    elif echo $cel | grep -q -E "^moyenne\(.+,.+\)$"
	then
	set - `separeMoteur $cel 7`
	set - `get_selection $1 $2`
	# On ne doit pas avoir d'erreurs dans le calcul des cases
	if echo "$*" | grep -q -E "#RTFM#"
	    then
	    echo "#RTFM# moyenne impossible : valeur(s) non numérique"
	else
	   	echo `moyenne $*`
	fi

    # variance(cel1,cel2)
    elif echo $cel | grep -q -E "^variance\(.+,.+\)$"
	then
	set - `separeMoteur $cel 8`
	set - `get_selection $1 $2`
	# On ne doit pas avoir d'erreurs dans le calcul des cases
	if echo "$*" | grep -q -E "#RTFM#"
	    then
            #affiche l'erreur obtenue dans la selection 
	    echo "#RTFM# variance impossible : valeur(s) non numérique"
	else
	    echo `variance $*`
	fi	

    # ecarttype(cel1,cel2)
    elif echo $cel | grep -q -E "^ecarttype\(.+,.+\)$"
	then
	set - `separeMoteur $cel 9`
	set - `get_selection $1 $2`
	# On ne doit pas avoir d'erreurs dans le calcul des cases
	if echo "$*" | grep -q -E "#RTFM#"
	    then	   
	    echo "#RTFM# ecarttype impossible : valeur(s) non numérique"
	else
	    var1=`variance $*`
	    if is_real_ge $var1
		then
		echo "sqrt(`variance $*`)" | bc -l
	    else
		echo "#RTFM# variance négative dc eccartype impossible"
	    fi
	fi

    # mediane(cel1,cel2)
    elif echo $cel | grep -q -E "^mediane\(.+,.+\)$"
	then
	set - `separeMoteur $cel 7`
	set - `get_selection $1 $2`
        # On ne doit pas avoir d'erreurs dans le calcul des cases
	if echo "$*" | grep -q -E "#RTFM#"
	    then	   
	    echo "#RTFM# mediane impossible : valeur(s) non numérique"
	else
	    #milieu de la liste
	    milieu=`echo " $# / 2" | bc -l` 
	    reste=`echo "$milieu" | cut -d "." -f2 | cut -c1`
	    milieu=`echo "$milieu" | cut -d "." -f1`
	    
	    #utilisation de "sort" pour trier.
	    rm -f ~/tmp/temporairetableurgrylassalle
	    while test $# -ge 1
	      do
	      echo $1 >> ~/tmp/temporairetableurgrylassalle
	      shift 
	    done
	    saveIFS=$IFS
	    IFS="
"
	    set - `sort ~/tmp/temporairetableurgrylassalle` 
	    `rm -f ~/tmp/temporairetableurgrylassalle`
	    
	    #s'il n'y a pas de reste (pair), on met reste a 1
	    #pour ne pas faire "shift" une fois de trop dans la liste
	    if test $reste -eq 0
		then
		reste=1
	    else
		reste=0
	    fi
            #on recherche la valeur du milieu
	    shift $[ $milieu - $reste ]
	    
            #si le nb du milieu est impair, alors on prend celui-ci
	    if test "$reste" -eq 0
		then
		echo "$1" 
	    else
	    #sinon on prend la moyenne de ceux sup et inf du milieu
		echo "( $1 + $2 ) / 2 " | bc -l
	    fi
	    IFS=$saveIFS
	fi

    # min(cel1,cel2)
    elif echo $cel | grep -q -E "^min\(.+,.+\)$"
	then
	set - `separeMoteur $cel 3`
	set - `get_selection $1 $2`
        # On ne doit pas avoir d'erreurs dans le calcul des cases
	if echo "$*" | grep -q -E "#RTFM#"
	    then
	    echo "#RTFM# min impossible : valeur(s) non numérique"
	else
	    min=$1
	    shift
	    while test $# -ge 1
	      do
	      res=$1
	      if [ $res -lt $min ]
		  then
		  min=$res
	      fi
	      shift
	    done
	    echo "$min"
	fi
	
    # max(cel1,cel2)
    elif echo $cel | grep -q -E "^max\(.+,.+\)$"
	then
	set - `separeMoteur $cel 3`
	set - `get_selection $1 $2`
        # On ne doit pas avoir d'erreurs dans le calcul des cases
	if echo "$*" | grep -q -E "#RTFM#"
	    then
	    echo "#RTFM# max impossible : valeur(s) non numérique"
	else
	    max=$1
	    shift
	    while test $# -ge 1
	      do
	      res=$1
	      if [ $res -gt $max ]
		  then
		  max=$res
	      fi
	      shift
	    done
	    echo "$max"
	fi
	
    # concat(val1,val2)
	elif echo $cel | grep -q -E "^concat\(.+,.+\)$"
	then
	set - `separeMoteur $cel 6`
	echo "`moteur $1``moteur $2`"

    # length(val1)
    elif echo $cel | grep -q -E "^length\(.+\)$"
	then
	set - `separeMoteur $cel 6`
	echo $[ `echo "$1" | wc -c` - 1 ]

    # substitute(val1,val2,val3)
    elif echo $cel | grep -q -E "^substitute\(.+,.+,.+\)$"
	then	
	set - `separeSubstitute $cel 10`
	echo "$1" | sed s/$2/$3/g

    # size(val1)
    elif echo $cel | grep -q -E "^size\(.+\)$"
	then
	set - `separeMoteur $cel 4`
	var1=`moteur $1`
	# $1 doit etre un fichier
	if [ -f "$var1" ]
	    then
	    echo `cat "$var1" | wc -c`
	else
	    echo "#RTFM# : $1 n'est pas un fichier"
	fi

    # lines(val1)
    elif echo $cel | grep -q -E "^lines\(.+\)$"
	then
	set - `separeMoteur $cel 5`
	# $1 doit etre un fichier
	if [ -f "$1" ]
	    then
	    echo `cat "$1" | wc -l`
	else
	    echo "#RTFM# : $1 n'est pas un fichier"
	fi

    # shell(val1)
    elif echo $cel | grep -q -E "^shell\(.+\)$"
	then
	cmde=`echo $cel | cut -c7- | sed "s/)$//"`
	$cmde > /dev/null &>/dev/null
	# $cmde doit etre une commande valide
	if [ $? -ne 0 ]
	    then
	    echo "#RTFM# : $cmde n'est pas une commande valide"
	else
	    $cmde
	fi


    # Tout le reste = chaine de caractère ou nombre etc....
    # CAD que ce n'est pas une formule à traiter
    else
	echo $cel
    fi
}

##-----------------------------------
## get_selection
##-----------------------------------
# IN : Deux cellules de la forme [licj]
# OUT : Une liste correspondant à toutes les cellules formé
#       par l'intervalle passé en paramètre
# Retour :
function get_selection
{
    local debut
    local fin
    local cel1
    local cel2
    local liste
    local contenu
    local x1
    local y1
    local x2
    local y2
    local sy
    local saveIFS
    local cellule
    liste=""

    # Le param 1 doit etre de la forme licj
    if echo $1 | grep -q -E "^l[0-9]+c[0-9]+$"
	then

        # Le param 2 doit etre de la forme licj
	if echo $2 | grep -q -E "^l[0-9]+c[0-9]+$"
	    then

	    # On récupère i et j de chacune des cellules
	    cel1=$1
	    cel2=$2
	    saveIFS=$IFS
	    IFS=" "
	    debut=`ij $cel1`
	    fin=`ij $cel2`
	    set $debut
	    x1=$1
	    y1=$2
	    sy=$y1
	    set $fin
	    x2=$1
	    y2=$2

	    IFS=$saveIFS

	    # Le x de départ doit etre plus petit que le x de fin
	    if [ $x2 -lt $x1 ]
		then
		echo "#RTFM# : x1>x2"

	    # Le y de départ doit etre plus petit que le y de fin
	    elif [ $y2 -lt $y1 ]
		then
		echo "#RTFM# : y1>y2"

	    # Tous les tests sont OK
	    else
		# On parcours toutes les lignes
		while test $x1 -le $x2
		  do
		  # Pour chacune des lignes, on parcours toutes les colones
		  while test $y1 -le $y2
		    do
		    # Premier élément de la liste :
		    # pas besoin d'afficher le séparateur
		    if [ "$liste" = "" ]
			then
			cellule="`cel $x1 $y1`"
			contenu=`analyse_debut $cellule`
			if is_real $contenu
			    then
			    liste="$contenu"
			else
			    liste="#RTFM# une cellule n'est pas un réel"
			fi
		    else
			cellule=`cel $x1 $y1`
			contenu=`analyse_debut $cellule`
			if is_real $contenu
			    then
			    liste="$liste $contenu"
			else
			    #erreur: ancienne valeurs sans importances
			    liste="#RTFM# une cellule n'est pas un réel"
			fi
		    fi
		    y1=$[ $y1 + 1 ]
		  done
		  y1=$sy
		  x1=$[ $x1 + 1 ]
		done
		echo $liste
	    fi ## Fin ts les tests OK
	else
	    echo "#RTFM# : cellule de fin non valide"
	fi ## Fin param 2 correct
    else
	echo "#RTFM# : cellule de debut non valide"
    fi ## Fin param 1 correct
}


##-----------------------------------
## analyse_debut
##-----------------------------------
# IN : Le contenu d'une cellule
# OUT : Le résultat calculé de la cellule concernée
# Retour :
# Fait un embranchement conditionnel suivant le contenu de la 
# cellule passée en paramètre.
# Cette fonction est appelée pour chacune des cellules de 
# la feuille de calcul
function analyse_debut
{
    local param
    local lacase
    local cel
    local fre
    local saveIFS
    local op
    op="\+|\-|\*|\/|somme|\^|ln|e|sqrt|somme|moyenne|variance|ecarttype|mediane|min|max|concat|length|substitute|size|lines|display"
    
    # On a une référence vers une autre cellule de forme : [licj]
    # Appel de moteur
    if echo $1 | grep -q -E "^=\[l[0-9]+c[0-9]+\]$"
	then
	param=`echo $1 | sed "s/^=//"`
	cel=`echo $param | cut -d "[" -f2 | cut -d "]" -f1`
	saveIFS=$IFS
	IFS=" "
	lacase=`ij $cel`
	IFS=$saveIFS
	fre=`cel $lacase`
	echo `analyse_debut "$fre"`

    # On a une formule qui commence par un =
    # Appel de moteur
    elif echo $1 | grep -q -E "^=.*"
	then
	param=`echo $1 | sed "s/^=//"`
	#if echo $param | grep -q -E "(somme|moyenne|size|shell|\/){1}.*"
	#    then
	    echo `moteur "$param"`
	#else
	#    echo "#RTFM# $param : opérateur non reconnu"
	#fi

    # Tout le reste est considéré comme du texte
    # Donc on l'affiche tout simplement
    else
	echo "$1"
    fi
}


##-----------------------------------
## parcours_feuille
##-----------------------------------
# IN : Aucun paramètre. On utilise les variables issues de l'analyse
#      des paramètres passés à ce programme
# OUT : La feuille calculée :)
# Retour :
function parcours_feuille
{
    local ligne
    local saveIFS
    local saveIFS2
    local ligne
    local cur_cel
    local nb

    saveIFS1=$IFS
    IFS=$slin
    set $fichier

    # Parcours de toutes les lignes
    for ligne in $*
      do

      saveIFS2=$IFS
      IFS=$scin
      set $ligne
      nb=$#

      # Parcours de toutes les colones de la ligne courante
      for cur_cel in $*
	do
	nb=$[ $nb - 1 ]
	IFS=$saveIFS1

	#### Calcul de la cellule courante
	echo -n `analyse_debut "$cur_cel"`

	IFS=$scin

	# On doit afficher $scout si on est en fin de ligne
	if [ $nb -ne 0 ]
	    then
	    echo -n "$scout"
	fi
      done
      
      IFS=$saveIFS2
      echo -n "$slout"
    done

    IFS=$saveIFS1
}


##-----------------------------------
## parcours_display
##-----------------------------------
# IN :
# OUT :
# Retour :
function parcours_display
{
    local liste
    local debut
    local fin
    local x1
    local y1
    local x2
    local y2
    local sy
    local saveIFS1
    local saveIFS2
    local saveListe
    local nb
    local savePara

    set $1

    # Tant qu'il y a des displays à afficher
    while test $# -gt 0
      do
      debut=`ij $1`
      fin=`ij $2`
      shift 2
      saveListe=$*
      set $debut
      x1=$1
      y1=$2
      sy=$y1
      set $fin
      x2=$1
      y2=$2

      # Le x de départ doit etre plus petit que le x de fin
      if [ $x2 -lt $x1 ]
	  then
	  echo "#RTFM# : x1>x2"
	  
      # Le y de départ doit etre plus petit que le y de fin
      elif [ $y2 -lt $y1 ]
	  then
	  echo "#RTFM# : y1>y2"
	  
      # Tous les tests sont OK
      else
	  saveIFS1=$IFS
	  IFS=$slin
	  set $fichier
	  shift `expr $x1 - 1`
	  # On parcours toutes les lignes
	  while test $x1 -le $x2
	    do
	    savePara=$*
	    IFS=$scin
	    set $1
	    shift `expr $y1 - 1`
	    nb=$[ $[ $y2 - $y1 ] + 1 ]
	    # Pour chacune des lignes, on parcours toutes les colones
	    while test $y1 -le $y2
	      do
	      nb=$[ $nb - 1 ]

	      #### Calcul de la cellule courante
	      IFS=$saveIFS1
	      echo -n `analyse_debut "$1"`
	      IFS=$scin

	      # On doit afficher $scout si on est en fin de ligne
	      if [ $nb -ne "0" ]
		  then
		  echo -n "$scout"
	      fi
	  
	      shift
	      y1=$[ $y1 + 1 ]
	    done
	    echo -n "$slout"

	    IFS=$slin
	    set $savePara
	    shift

	    y1=$sy
	    x1=$[ $x1 + 1 ]
	  done
      fi
      IFS=" "
      set -- $saveListe
    done
}


##-----------------------------------
## make_world
##-----------------------------------
# IN :
# OUT :
# Retour :
function make_world
{
    local saveIFS1
    local saveIFS2
    local liste
    local cur_cel
    local ligne

    if echo $fichier | grep -q "=display"
	then
	#echo "recherche des displays"
	saveIFS1=$IFS
	IFS=$slin
	set $fichier
	
        # Parcours de toutes les lignes
	for ligne in $*
	  do
	  
	  saveIFS2=$IFS
	  IFS=$scin
	  set $ligne
	  
          # Parcours de toutes les colones de la ligne courante
	  for cur_cel in $*
	    do
	    IFS=$saveIFS1
	    
	    #### Calcul de la cellule courante
	    if echo $cur_cel | grep -q -E "^=display\(l[0-9]+c[0-9]+,l[0-9]+c[0-9]+\)$"
		then
		liste="$liste `separeMoteur $cur_cel 8`"
	    fi
	    
	    IFS=$scin
	  done
	  
	  IFS=$saveIFS2
	done

	IFS=$saveIFS1
	parcours_display "$liste"

    else
	# pas de display. Parcours de toute la feuille
	parcours_feuille
    fi
}


##-----------------------------------
## inverse
##-----------------------------------
# IN : la feuille calculée
# OUT : feuille calculée avec inversion lignes/colonnes
# Retour :
function inverse
{
    local saveIFS
    local nb
    local seek
    local i
    seek=$1
    saveIFS=$IFS
    IFS="$slout$scout"
    set $seek
    nb=$#
    echo "nb: $nb"

    while test $nb -gt 0
      do
      IFS=$slout
      set -- $seek
      seek=""
      #parcours de toutes lignes
      for i in $*
	do
	savePara=$*
	IFS=$scin
	set $i
	echo -n "$1$scout"
	shift
	seek="$seek$slout$*"
	IFS=$slout
	set $savePara
	nb=$[ $nb - 1 ]
      done
      echo $slout
    done
    IFS=$saveIFS
}



#####################################
# Variable pr récupérer les paramètres
#####################################
# Feuille en entrée
in="/dev/stdin"

# Feuille en sortie
out="/dev/stdout"

# Séparateur de colone en entrée, par défaut tabulation
scin="	"

# Séparateur de ligne en entrée, par défaut, retour chariot
slin="
"

# Séparateur de colone en sortie
scout=""

# Séparateur de ligne en sortie
slout=""

# Invertion des lignes/colones en sortie. PAr défaut non (0)
inverse="0"

# Marqueur si scout non spécifié
bool_scout="0"

# Marqueur si slout non spécifié
bool_slout="0"


############################
# Récupération des paramètres
############################
while [ $# -ne 0 ]
  do
  case $1 in
      "-in")
	  if [ $# -lt 2 ]
	      then
	      echo "pb: -in"
	      exit 0
	  else
	      in=$2
	  fi
	  if [ ! -f "$in" ]
	      then
	      help 2
	  fi
	  shift 2
	  ;;
      "-out")
	  if [ $# -lt 2 ]
	      then
	      echo "pb: -out"
	  else
	      out=$2
	  fi
	  shift 2
	  ;;
      "-scin")
	  if [ $# -lt 2 ]
	      then
	      echo "pb: -scin"
	  else
	      scin=$2
	  fi
	  shift 2
	  ;;
      "-scout")
	  bool_scout="1"
	  if [ $# -lt 2 ]
	      then
	      echo "pb: -scout"
	  else
	      scout=$2
	  fi
	  shift 2
	  ;;
      "-slin")
	  if [ $# -lt 2 ]
	      then
	      echo "pb: -slin"
	  else
	      slin=$2
	  fi
	  shift 2
	  ;;
      "-slout")
	  bool_slout="1"
	  if [ $# -lt 2 ]
	      then
	      echo "pb: -slout"
	  else
	      slout=$2
	  fi
	  shift 2
	  ;;
      "-inverse")
	  inverse="1"
	  shift
	  ;;
      "-h")
	  help
	  ;;
      *)
	  echo "erreur, paramètre inconu"
	  exit 2
	  ;;
  esac  
done

# Si on a pas spécifié slout, slout=slin
if [ $bool_slout = "0" ] 
    then
    slout=$slin
fi

# Si on a pas spécifié scout, slout=scin
if [ $bool_scout = "0" ] 
    then
    scout=$scin
fi

##
# Fin de récupération des paramètres
##################################

saveIFS=$IFS
IFS=""
fichier=`cat $in`
IFS=" "

tmp=`make_world`

if [ $inverse -eq "0" ]
    then
    echo $tmp > $out
else
    echo `inverse $tmp` > $out
fi

IFS=$saveIFS

debug=0

if [ "$debug" -eq 1 ]
    then
    echo "in: $in"
    echo "out: $out"
    echo "scin: $scin"
    echo "slin: $slin"
    echo "scout: $scout"
    echo "slout: $slout"
    echo "inverse: $inverse"
fi
