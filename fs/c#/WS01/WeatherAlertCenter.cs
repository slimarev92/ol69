using System;
using System.Collections.Generic;
using System.Text;

namespace ilrd.WS03
{
    public abstract class Traveler
    {
        public int NumOfTravelers { get; }
        public Park CurrentPark { get; set; }

        protected Traveler(int numOfTravelers = 1)
        {
            NumOfTravelers = numOfTravelers;
        }

        protected string GetParkDescription() => null != CurrentPark ? $", currently at {CurrentPark}" : String.Empty;

        public abstract void StormHandler(uint severity);
    }

    public abstract class TravelerGroup : Traveler
    {
        private readonly List<string> names = new List<string>();
        private readonly string groupTypeDescription;

        protected TravelerGroup(List<string> names, string groupTypeDescription) : base(names.Count)
        {
            this.names.AddRange(names);
            this.groupTypeDescription = groupTypeDescription;
        }

        public override string ToString()
        {
            var description = new StringBuilder();

            description.Append($"A {groupTypeDescription} with {names.Count} travelers{GetParkDescription()}:");

            foreach (string name in names)
            {
                description.Append($"\n# {name}");
            }

            return description.ToString();
        }
    }

    public class ElderlyTravelers : TravelerGroup
    {
        public ElderlyTravelers(List<string> names) : base(names, "group of elderly people") { }

        public override void StormHandler(uint severity)
        {
            CurrentPark.Leave(this);
        }
    }

    public class FamilyOfTravelers : TravelerGroup
    {
        public FamilyOfTravelers(List<string> names) : base(names, "family") { }

        public override void StormHandler(uint severity)
        {
            if (3 <= severity)
            {
                CurrentPark.Leave(this);
            }
            else
            {
                Console.WriteLine($"Staying in {CurrentPark} - {ToString()}");
            }
        }
    }

    public class SingleTraveler : Traveler
    {
        private readonly string name;

        public SingleTraveler(string name) : base()
        {
            this.name = name;
        }

        public override string ToString() => $"# A single traveler named {name}{GetParkDescription()}";

        public override void StormHandler(uint severity)
        {
            if (5 <= severity)
            {
                CurrentPark.Leave(this);
            }
            else
            {
                Console.WriteLine($"{name} is staying in {CurrentPark}");
            }
        }
    }

    public class Park
    {
        private readonly string name;
        public event Action<uint> StormAlertHandlers;

        public Park(string name) => this.name = name;

        public void Arrive(Traveler traveler)
        {
            StormAlertHandlers += traveler.StormHandler;
            Console.WriteLine($"Arrival at {name}:\n{traveler}");

            traveler.CurrentPark = this;
        }

        public void Leave(Traveler traveler)
        {
            StormAlertHandlers -= traveler.StormHandler;

            Console.WriteLine($"Leaving {name}:\n{traveler}");
        }

        public void RaiseStormAlert(uint severity)
        {
            if (10 < severity) { throw new ArgumentException("Severity is higher than maximum possible value", "severity"); }

            Console.WriteLine($"A storm with severity {severity} has begun!");
            StormAlertHandlers?.Invoke(severity);
        }

        public override string ToString()
        {
            return name;
        }
    }

    public class DayInThePark
    {
        /*  private static void Main()
          {
              var park = new Park("Sasha's Park");
              var single = new SingleTraveler("Yasmin");
              var family = new FamilyOfTravelers(new List<string>() { "Eldad", "Tami" });
              var elderly = new ElderlyTravelers(new List<string>() { "Ella", "Vladimir" });

              park.Arrive(single);
              Console.WriteLine("*****************************************************");

              park.Arrive(family);
              Console.WriteLine("*****************************************************");

              park.RaiseStormAlert(1);
              Console.WriteLine("*****************************************************");

              park.Arrive(elderly);
              Console.WriteLine("*****************************************************");

              park.RaiseStormAlert(2);
              Console.WriteLine("*****************************************************");

              park.RaiseStormAlert(3);
              Console.WriteLine("*****************************************************");

              park.Arrive(family);
              Console.WriteLine("*****************************************************");

              park.RaiseStormAlert(8);
              Console.WriteLine("*****************************************************");

              park.RaiseStormAlert(100); // This should throw an exception
              Console.WriteLine("*****************************************************");
          }*/
    }
}