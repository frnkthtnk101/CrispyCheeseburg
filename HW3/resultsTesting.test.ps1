Import-Module Pester
cd C:\Users\fpettigrosso\ws\CrispyCheeseburg\HW3
$correct = Get-Content .\correct.txt
$results = Get-Content .\results.txt
$ValuesCorrect = @()
$ValuesResults = @()
$correct | ForEach-Object {
    $ValuesCorrect += $_.Split(" ")[-1]
}
$results | ForEach-Object {
    $ValuesResults += $_.Split(" ")[-1]
}
cls
Describe "Test"{
    Context "testing line value"{
    for($i=0; $i -lt $correct.Count; $i++){
    it "$($correct[$i]) vs $($results[$i])"{
         $ValuesResults[$i] |Should be $ValuesCorrect[$i]
        }
    }
    }
}