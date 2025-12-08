


	public:
		HttpRequest	httpRequest();
	
	private:
		void		parseHttpRequest(int clientFd);
		std::string	parseMethod(std::string method);
		HttpVersion	parseVersion(std::string version);
		