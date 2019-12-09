using System;

namespace at.jku.ssw.Coco.tools {

  public class CSParser {
    public static void Main (string[] args) {
      Console.WriteLine("_________________________________");
      Console.WriteLine("CS2Parser (HM, AW, ML, September 2005)");

      if (args.Length < 1)
        Console.WriteLine("Syntax : CS2Parser <C# source file> { <conditional compilation symbol> }");
      else {
        Console.WriteLine("   Initializing scanner with source file {0}", args[0]);
        Scanner scanner = new Scanner(args[0]);
        Parser parser = new Parser(scanner);
        if (args.Length > 1) {
          Console.WriteLine("   Initializing parser with conditional compilation symbols");
          String[] ccs = new String[args.Length-1];
          System.Array.Copy(args, 1, ccs, 0, ccs.Length);
          parser.AddConditionalCompilationSymbols(ccs);
        }
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
