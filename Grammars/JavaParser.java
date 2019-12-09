// import classes Parser, Sanner and Errors if not in the same package as
// JavaParser
public class JavaParser {

	public static void main(String[] args) {
	    System.out.println("___________________________");
	    System.out.println("JavaParser (AH, June 2004)");

	    if (args.length > 0) {
		System.out.println("   Reading source file " + args[0]);
		Scanner scanner = new Scanner(args[0]);
		System.out.println("   Parsing source file " + args[0]);
		Parser parser = new Parser(scanner);
		parser.Parse();

		if (parser.errors.count == 1)
		    System.out.println("-- 1 error dectected");
		else
		    System.out.println("-- " + parser.errors.count + " errors dectected");

	    }
	    else
		System.out.println("Syntax: JavaParser <java source file>");

	}

} // JavaParser
