using ilrd.WS02;
using NUnit.Framework;

namespace ilrd.Tests
{
    public class ShapeTest
    {
        [Test]
        public void TestRectangle()
        {
            var square = new Rectangle();

            double expectedArea = 1.0;
            double actualArea = square.Area();

            Assert.AreEqual(expectedArea, actualArea);

            double expectedPerimeter = 4.0;
            double actualPerimeter = square.Perimeter();

            Assert.AreEqual(expectedPerimeter, actualPerimeter);
        }

        [Test]
        public void TestComplex()
        {
            var complex = new ComplexShape();

            complex.Add(new Rectangle());
            complex.Add(new Rectangle());
            complex.Add(new Rectangle());
            complex.Add(new Rectangle());

            double expectedArea = 4.0;
            double actualArea = complex.Area();

            Assert.AreEqual(expectedArea, actualArea);

            double expectedPerimeter = 16.0;
            double actualPerimeter = complex.Perimeter();

            Assert.AreEqual(expectedPerimeter, actualPerimeter);
        }
    }
}
