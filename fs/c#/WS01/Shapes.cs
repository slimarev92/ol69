using System.Collections.Generic;

namespace ilrd.WS02
{
    public abstract class Shape
    {
        public string Color { get; set; }
        public bool IsFilled { get; set; }
        private readonly string filled = "is";
        private readonly string notFilled = "is not";


        public Shape(string color, bool isFilled)
        {
            Color = color;
            IsFilled = isFilled;
        }

        public Shape() : this("green", true) { }

        static Shape()
        {
            System.Console.WriteLine("Shape initialized");
        }

        public abstract double Area();
        public abstract double Perimeter();

        public override string ToString()
        {
            string filledString = IsFilled ? filled : notFilled;

            return "A shape with color " + Color + " that " + filledString + " filled.";
        }
    }

    public class Rectangle : Shape
    {
        public double Width { get; set; }
        public double Length { get; set; }

        public Rectangle(double width = 1.0, double lenght = 1.0)
        {
            Width = width;
            Length = lenght;
        }

        public Rectangle(string color, bool isFilled, double width, double length)
            : base(color, isFilled)
        {
            Width = width;
            Length = length;
        }

        static Rectangle()
        {
            System.Console.WriteLine("Rectangle initialized");
        }

        public override double Area()
        {
            return Width * Length;
        }

        public override double Perimeter()
        {
            return (Width * 2) + (Length * 2);
        }

        public override string ToString()
        {
            return "A rectangle with width = " + Width + " and length = " + Length
                + " which is derived from: " + base.ToString();
        }
    }

    public class Circle : Shape
    {
        public double Radius { get; set; }

        public Circle(double radius = 1.0)
        {
            Radius = radius;
        }

        public Circle(string color, bool isFilled, double radius) : base(color, isFilled)
        {
            Radius = radius;
        }

        public override double Area()
        {
            return System.Math.PI * Radius * Radius;
        }

        public override double Perimeter()
        {
            return 2.0 * System.Math.PI * Radius;
        }

        public override string ToString()
        {
            return "A circle with radius = " + Radius + ", that is derived from: " + base.ToString();
        }
    }

    public class ComplexShape : Shape
    {
        private readonly List<Shape> shapes = new List<Shape>();

        public void Add(Shape shape) => shapes.Add(shape);

        public override double Area()
        {
            double total = 0;

            foreach (Shape shape in shapes)
            {
                total += shape.Area();
            }

            return total;
        }

        public override double Perimeter()
        {
            double total = 0;

            foreach (Shape shape in shapes)
            {
                total += shape.Perimeter();
            }

            return total;
        }
    }
}
