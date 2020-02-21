using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace LearnEF
{
    [Table("Inventory")]
    public class InventoryItem
    {
        [Key]
        public int InventoryId { get; set; }
        public int Price { get; set; }
        public int Quantity { get; set; }

        [ForeignKey("Type")]
        public int ProductTypeId { get; set; }

        public ProductTypeInfo Type { get; set; }
    }
}
