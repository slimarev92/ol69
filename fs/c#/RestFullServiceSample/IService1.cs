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
    [ServiceContract]
    public interface IService1
    {
        [OperationContract]
        [WebInvoke(
          Method = "POST",
          RequestFormat = WebMessageFormat.Json,
          ResponseFormat = WebMessageFormat.Json,
          UriTemplate = "SetTodoItems")]
        void SetItems(List<TodoItem> items);

        [OperationContract]
        [WebGet(
           RequestFormat = WebMessageFormat.Json,
           ResponseFormat = WebMessageFormat.Json,
           UriTemplate = "GetTodoItems")]
        List<TodoItem> GetItems();

        [OperationContract]
        [WebInvoke(
         Method = "POST",
         RequestFormat = WebMessageFormat.Json,
         ResponseFormat = WebMessageFormat.Json,
         UriTemplate = "RemoveItems")]
        void RemoveItems(List<TodoItem> items);

        [OperationContract]
        [WebGet(
           RequestFormat = WebMessageFormat.Json,
           ResponseFormat = WebMessageFormat.Json,
           UriTemplate = "RequestId")]
        ID RequestId();
    }
}
