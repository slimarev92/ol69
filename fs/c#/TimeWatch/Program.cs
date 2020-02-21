using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HtmlAgilityPack;

namespace TimeWatch
{
    class Program
    {
        public static void ConvertTimeWatch(string sourceFile, string destFile)
        {
            var doc = new HtmlDocument();

            doc.OptionDefaultStreamEncoding = Encoding.UTF8;

            doc.Load(sourceFile);

            //select table with work hours
            var table =
                doc.DocumentNode.SelectNodes("//table").ToList()[2];
            var tableBody = doc.DocumentNode.SelectNodes("//tbody").ToList()[2];

            tableBody.SetAttributeValue("id", "main-table");
            var rows = tableBody.ChildNodes.ToList();

            //remove header rows
            for(int i = 0; i < 6; ++i)
            {
                rows[i].Remove();
            }

            //remove footer rows
            for (int i = 0; i < 6; ++i)
            {
                rows[rows.Count -1 - i].Remove();
            }

            var destDoc = new HtmlDocument();

            //remove everything from body element
            doc.DocumentNode.RemoveAllChildren();

            //creat rtl div (eww), insert table into the div
            var node = new HtmlNode(HtmlNodeType.Element, doc, 0);
            node.Name = "div";
            node.SetAttributeValue("dir", "rtl");
            node.AppendChild(table);

            //insert the div into the document
            doc.DocumentNode.AppendChild(node);

            //save document
            doc.Save(destFile);
            Console.WriteLine("Finished Writing to destination file.");
        }

        static void Main(string[] args)
        {
            ConvertTimeWatch(@"C:\Users\varagor\Desktop\tw.html", @"C:\Users\varagor\Desktop\tw2.html");
        }
    }
}
