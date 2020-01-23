using System;
using System.Xml;
using System.IO;
using System.Text;

namespace XMLToText
{
    class Program
    {
        public static void WriteToDest(string source, string dest)
        {
            var settings = new XmlReaderSettings() { IgnoreWhitespace = true };
            using var document = XmlReader.Create(source, settings);
            var destFile = new FileInfo(dest);
            using var destWriter = new StreamWriter(destFile.OpenWrite());

            while (true == document.Read())
            {
                if ("member" == document.Name && (document.GetAttribute("name")?.StartsWith("M") ?? false))
                {
                    var currEntry = new StringBuilder();

                    currEntry.Append(document.GetAttribute("name").Replace("\n", string.Empty).Trim());
                    currEntry.Append("\n");

                    var tag = document.ReadSubtree();
                    // Read once to advance past the root element.
                    tag.Read();

                    while (true == tag.Read())
                    {
                        if (!tag.Name.Equals("member"))
                        {
                            currEntry.Append($"{tag.Name}: ");
                            currEntry.Append(tag.ReadString().Replace("\n", string.Empty).Trim());
                            currEntry.Append("\n");
                        }
                    }

                    currEntry.Append("****************************************************************************************\n");

                    Console.Write(currEntry.ToString());
                    destWriter.Write(currEntry.ToString());
                }
            }
        }

        public static void Main(string[] args)
        {
            if(args.Length < 2) { throw new ArgumentException("XML file path is missing", "args"); }

            WriteToDest(args[0], args[1]);
        }
    }
}
