Import-Module Pester
cd C:\Users\fpettigrosso\ws\CrispyCheeseburg\HW3
$correct = Get-Content .\correct.txt
$results = Get-Content .\results.txt
cls
Describe "testing results"{

    Context "Testing lines"{
            $i = 0
            $t =0
        foreach( $line in $results){
            it "$line" {


                $correct.Contains($line) | Should be $true


            }
 
        }
               
    }
}
