using System;

namespace at.jku.ssw.Coco.tools {

    public class CSParser {
	public static void Main (string[] args) {
	    Console.WriteLine("_______________________________");
	    Console.WriteLine("CSP (HM, AW, ML, June 2005)");

	    if (args.Length < 1)
		Console.WriteLine("Syntax : csp <C# source file>");
	    else {
		Console.WriteLine("   Reading source file {0}", args[0]);
		Scanner scanner = new Scanner(args[0]);
		Parser parser = new Parser(scanner);
		Console.WriteLine("   Parsing source file {0}", args[0]);
		parser.Parse();
		if (parser.errors.count == 1)
		    Console.WriteLine("-- 1 error dectected");
		else
		    Console.WriteLine("-- {0} errors dectected", parser.errors.count);
	    }
	}
    }

}
