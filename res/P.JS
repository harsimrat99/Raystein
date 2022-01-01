/*
s = 10

for (i = 0; i < s; i++) {


	for (j = 0; j < s; j++) {

	
		if (j % 2 ) {

			process.stdout.write('W');

		}

		else process.stdout.write('O');
		

	}

	console.log();

}

*/

// Prints diamond pattern with 2n rows
      function printDiamond(n) {
        var space = n - 1;
 
        // run loop (parent loop)
        // till number of rows
        for (var i = 0; i < n; i++) {
          // loop for initially space,
          // before star printing
          for (var j = 0; j < space; j++) process.stdout.write("O");
 
          // Print i+1 stars
          for (var j = 0; j <= i; j++) process.stdout.write("W" + "O");
 
          console.log();
          space--;
        }
 
        // Repeat again in reverse order
        space = 0;
 
        // run loop (parent loop)
        // till number of rows
        for (var i = n; i > 0; i--)
        {
         
          // loop for initially space,
          // before star printing
          for (var j = 0; j < space; j++) process.stdout.write("O");
 
          // Print i stars
          for (var j = 0; j < i; j++) process.stdout.write("W" + "O");
 
          console.log();
          space++;
        }
      }
 
      // Driver code
      printDiamond(5);