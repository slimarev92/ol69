using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using TodoItems;

namespace RestFullServiceSample
{
    [ServiceBehavior(IncludeExceptionDetailInFaults = true)]
    public class Service1 : IService1
    {
        public List<TodoItem> GetItems()
        {
            var userItems = new ItemAccess();

            return userItems.GetUserItems();

            /*
            var list = new List<TodoItem>()
            {
                new TodoItem(1, "abcd", true)
            };
            GetHighestId()
            return list;
            */
        }

        public ID RequestId()
        {
            var userItems = new ItemAccess();

            return new ID(userItems.GetHighestId());
        }

        public void SetItems(List<TodoItem> todoItems)
        {
            var userItems = new ItemAccess();

            userItems.AddItems(todoItems);
        }

        public void RemoveItems(List<TodoItem> items)
        {
            var userItems = new ItemAccess();

            userItems.RemoveItems(items);
        }
    }
}
