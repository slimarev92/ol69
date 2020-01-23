using System.Collections.Generic;
using System.ComponentModel;
using System.ServiceModel;
using System.ServiceModel.Web;

namespace ilrd
{
    [ServiceContract]
    public interface ITodoService
    {
        [OperationContract]
        [WebGet(
           RequestFormat = WebMessageFormat.Json,
           ResponseFormat = WebMessageFormat.Json,
           UriTemplate = "GetTodoItems")]
        List<TodoItem> GetItems();
    }
}
