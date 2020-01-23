using System.Runtime.Serialization;
using System.ServiceModel;

namespace ilrd.WCF
{
    [ServiceContract(Namespace = "http://ilrd.WCF.CalcService")]
    public interface ICalculator
    {
        [OperationContract]
        int Add(int operand1, int operand2);

        [OperationContract]
        int Substract(int operand1, int operand2);
    }
}
