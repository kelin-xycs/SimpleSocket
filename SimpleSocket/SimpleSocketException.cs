using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimpleSocket
{
    public class SimpleSocketException : Exception
    {
        private int errorCode;

        public SimpleSocketException(string message, int errorCode) : base(message)
        {
            this.errorCode = errorCode;
        }

        public int ErrorCode
        {
            get { return this.errorCode; }
        }
    }
}
