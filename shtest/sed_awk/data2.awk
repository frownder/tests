BEGIN { OFS="\t" 
        print "NAME \t VALUE"
        print "------------------------"
      }
      {printf "%-10s %10s %5d\n", $1,$3,$7
       total += $7}
      /north/{count++}
END { print "--------------------------"
      print "total: "total "north count:" count
    }
