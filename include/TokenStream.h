



readObject<Matrix>(TokenStream("camera1.txt"), &cameraMatrix);

bool readFloat(TokenStream& tokenStream, float& f)
{
	if(tokenStream.curTokenType() != NumberToken){
		reportError("Expected number");
	}
}

template<>
bool readObject<Vector>(TokenStream& tokenStream, Vector& v)
{
	if(tokenStream.curTokenType() != BeginArrayToken){
		reportError("Expected array");
	}
	tokenStream.beginArray();
		readFloat(tokenStream, v.x);
		readFloat(tokenStream, v.y);
		readFloat(tokenStream, v.z);
	tokenStream.endArray();
}

template<>
bool readObject<Matrix>(io::TokenStream& tokenStream, Matrix& m)
{
	if(tokenStream.curTokenType() != io::TokenStream::ObjectToken){
		reportError("Current token is not object");
		return false;
	}
	io::ObjectReader objectReader(tokenStream);
	const int OriginField = 1;
	const int AxesField = 2;
	const int AllFields = OriginField | AxesField;
	int fields = 0;
	while(true){
		objectReader.readNextField();
		if(error){
			break;
		}
		if(objectReader.curFieldName() == "origin"){
			readObject<Vector>(tokenStream, m.origin);
			if(error){
				break;
			}
			fields |= OriginField;
		}else if(objectReader.curFieldName() == "axes"){
			readObject<Axes>(tokenStream, m.axes);
			if(error){
				break;
			}
			fields |= AxesField;
		}
		if(fields == AllFields){
			break;
		}
	}
	return (fields == AllFields);
}

matrix.axes.x.x

matrix
	axes
		x
			x y z
		y
			x y z
		z
			x y z
	origin