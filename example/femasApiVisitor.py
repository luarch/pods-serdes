from api_femas import messages as FemasMessages
import json

def main():
    data = FemasMessages.CUstpFtdcRspUserLoginField()
    data.TradingDay = '20180101'
    data.UserID = '123'
    data.FemasLifeCycle = 100
    print("Before change: ", data)

    # Serialize
    j = json.dumps(vars(data))
    print("Serialized: ", j)

    # Deseiralize
    d = json.loads(j)
    d["UserID"] = "456"
    data = FemasMessages.CUstpFtdcRspUserLoginField(**d)
    print("After change: ", data)

if __name__ == "__main__":
    main()
