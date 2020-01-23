using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Data.Entity;
using System.Linq;

namespace TodoItems
{
    public class ID
    {
        public int id { get; set; }

        public ID() { }

        public ID(int id) => this.id = id;
    }

    public class TodoItem
    {
        [Key]
        public int Id { set; get; }

        [Column(TypeName = "VARCHAR")]
        [StringLength(30)]
        public string Content { set; get; }

        public bool Done { set; get; }

        public TodoItem()
        {

        }

        public TodoItem(int Id, string Content, bool Done)
        {
            this.Id = Id;
            this.Content = Content;
            this.Done = Done;
        }
    }

    [DbConfigurationType(typeof(MySql.Data.Entity.MySqlEFConfiguration))]
    public class TodoItemContext : DbContext
    {
        public TodoItemContext(string conStr) : base(conStr)
        {
        }

        public DbSet<TodoItem> Items { get; set; }
    }

    public class ItemAccess
    {
        public void AddItems(List<TodoItem> items)
        {
            var itemDB = new TodoItemContext("server=localhost;port=3306;database=todo;uid=sasha;password=996633");
            foreach (var item in items)
            {
                var itemInDB = itemDB.Items.SingleOrDefault(dbItem => dbItem.Id == item.Id);

                if (null == itemInDB)
                {
                    itemDB.Items.Add(item);
                }
                else
                {
                    itemInDB.Content = item.Content;
                    itemInDB.Done = item.Done;
                }
            }
            itemDB.SaveChanges();

            itemDB.Dispose();
        }

        public List<TodoItem> GetUserItems()
        {
            var itemDB = new TodoItemContext("server=localhost;port=3306;database=todo;uid=sasha;password=996633");

            var result = itemDB.Items.SqlQuery("select * from todoitems").ToList<TodoItem>();

            itemDB.Dispose();

            return result;
        }

        public void RemoveItems(List<TodoItem> items)
        {
            var itemDB = new TodoItemContext("server=localhost;port=3306;database=todo;uid=sasha;password=996633");

            foreach (var item in items)
            {
                var itemToRemove = itemDB.Items.SingleOrDefault(currItem => currItem.Id == item.Id);

                if (itemToRemove != null)
                {
                    itemDB.Items.Remove(itemToRemove);
                }
            }

            itemDB.SaveChanges();
            itemDB.Dispose();
        }

        public int GetHighestId()
        {
            var itemDB = new TodoItemContext("server=localhost;port=3306;database=todo;uid=sasha;password=996633");

            itemDB.Items.Add(new TodoItem(0, "", false));
            itemDB.SaveChanges();

            var result = itemDB.Items.SqlQuery("SELECT * from todoitems ORDER BY id DESC LIMIT 1").ToList<TodoItem>();

            itemDB.Dispose();

            return result[0].Id;
        }

        public void Save()
        {
            //itemDB.SaveChanges();
        }
    }
}



