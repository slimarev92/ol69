using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using ilrd.WCF;

[ServiceContract]
public interface ITodoItemsService
{
    [OperationContract]
    [WebGet(UriTemplate = "Items", ResponseFormat = WebMessageFormat.Json)]
    List<Item> GetUserItems();
}
