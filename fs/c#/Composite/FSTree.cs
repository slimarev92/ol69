using System;
using System.IO;

namespace ilrd.Composite
{
    public class FSTree
    {
        private readonly Branch rootDirectory;

        public FSTree(string directory)
        {
            DirectoryInfo dirInfo = new DirectoryInfo(directory);

            if (!dirInfo.Exists) { throw new System.ArgumentException("Folder doesn't exist."); }

            rootDirectory = new Branch(directory);
        }

        public void PrintTree()
        {
            rootDirectory.Print(0);
        }

        private class Branch : Leaf
        {
            private readonly IFSNode[] children;

            public Branch(string name) : base(name)
            {
                FileSystemInfo[] childrenInfo = new DirectoryInfo(name).GetFileSystemInfos();
                children = new IFSNode[childrenInfo.Length];

                for (int i = 0; i < children.Length; ++i)
                {
                    string childName = childrenInfo[i].FullName;
                    children[i] = (childrenInfo[i].Attributes & FileAttributes.Directory) != 0 ?
                        new Branch(childName) : new Leaf(childName);
                }
            }

            public override void Print(int depth)
            {
                base.Print(depth);

                foreach (IFSNode child in children)
                {
                    child.Print(depth + 1);
                }
            }
        }

        private class Leaf : IFSNode
        {
            private static readonly string LayerString = "   ";
            private static readonly string AngleString = "└──";

            private string Name { get; }

            public Leaf(string name)
            {
                Name = name;
            }

            public virtual void Print(int depth)
            {
                for (int i = 0; i < depth; ++i)
                {
                    Console.Write(LayerString);
                }

                Console.WriteLine(AngleString + Name);
            }
        }

        private interface IFSNode
        {
            public abstract void Print(int depth);
        }

        public static void Main()
        {
            FSTree tree = new FSTree("C:\\Users\\slima\\source\\repos\\sasha-limarev\\c");

            tree.PrintTree();
        }
    }
}
