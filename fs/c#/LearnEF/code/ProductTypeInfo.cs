using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace LearnEF
{
    [Table("ProductTypes")]
    public class ProductTypeInfo
    {
        [Key]
        public int Id { get; set; }

        [Required]
        public ProductType Type;

        [Required]
        public string Name { get; set; }
    }
}
