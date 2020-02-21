using System.ComponentModel.DataAnnotations.Schema;
using System.Data.Entity;

namespace LearnEF
{
    public class StoreContext : DbContext
    {
        public DbSet<InventoryItem> Inventory { get; set; }
        public DbSet<ProductTypeInfo> ProductTypes { get; set; }

        public DbSet<PersonalComputer> PersonalComputers { get; set; }

        public StoreContext() : base("mysqlDbConnection")
        {

        }
    }
}
