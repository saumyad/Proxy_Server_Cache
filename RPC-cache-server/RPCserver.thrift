namespace cpp RPC // The generated c++ code will be put inside example namespace

struct responseData {
	1: bool isSuccess
	2: string webPage
}

service RPCservice{
	// return the web page content requested
	responseData getWebPage(1:string url)
}