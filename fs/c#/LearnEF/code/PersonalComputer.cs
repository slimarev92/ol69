using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace LearnEF
{
    [Table("PersonalComputers")]
    public class PersonalComputer
    {
        [Key]
        public int Id { get; set; }

        public string Name { get; set; }

        [ForeignKey("Item")]
        public int ProductId { get; set; }

        [ForeignKey("Type")]
        public int ProductTypeId { get; set; }

        public ProductTypeInfo Type { get; set; }

        public InventoryItem Item { get; set; }
    }
}
