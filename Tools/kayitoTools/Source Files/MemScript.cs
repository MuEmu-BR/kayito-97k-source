using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace kayitoTools.Source_Files
{
	class MemScript
	{
		char[] m_buff;

		int m_size = 0;

		string m_path;

		int m_count;

		float m_number;

		readonly char[] m_string = new char[256];

		string m_LastError;

		private readonly string MEM_SCRIPT_ALLOC_ERROR = "Could not alloc memory for MemScript\n";

		private readonly string MEM_SCRIPT_ERROR_CODE0 = "{0} Could not open file\n";

		private readonly string MEM_SCRIPT_ERROR_CODE1 = "{0} Could not alloc file buffer\n";

		private readonly string MEM_SCRIPT_ERROR_CODE2 = "{0} Could not read file\n";

		private readonly string MEM_SCRIPT_ERROR_CODE3 = "{0} Could not get file buffer\n";

		private readonly string MEM_SCRIPT_ERROR_CODE4 = "{0} The file were not configured correctly\n";

		private readonly string MEM_SCRIPT_ERROR_CODEX = "{0} Unknow error code: {1}\n";

		public enum ETokenResult
		{
			TOKEN_NUMBER = 0,
			TOKEN_STRING = 1,
			TOKEN_END = 2,
			TOKEN_ERROR = 3,
		};

		public MemScript()
		{
			m_LastError = MEM_SCRIPT_ALLOC_ERROR;
		}

		public bool SetBuffer(string path)
		{
			m_path = path;

			SetLastError(4);

			try
			{
				if (!File.Exists(path))
				{
					throw new NullReferenceException(path + " not found.");
				}

				StreamReader reader = new StreamReader(path, Encoding.Default);

				m_size = (int)reader.BaseStream.Length;

				m_buff = new char[m_size];

				if (m_buff.Length == 0)
				{
					SetLastError(1);

					return false;
				}

				if (reader.Read(m_buff, 0, m_size) == 0)
				{
					SetLastError(2);

					return false;
				}

				m_count = 0;

				reader.Close();

				return true;
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);

				return false;
			}
		}

		char GetChar()
		{
			if (m_count >= m_size)
			{
				return '@';
			}

			return m_buff[m_count++];
		}

		void UnGetChar(char ch)
		{
			if (m_count == 0)
			{
				return;
			}

			m_buff[--m_count] = ch;
		}

		char CheckComment(char ch)
		{
			if (ch != '/' || (ch = GetChar()) != '/')
			{
				return ch;
			}

			while (true)
			{
				if ((ch = GetChar()) == '@')
				{
					return ch;
				}

				if (ch == '\n')
				{
					return ch;
				}
			}
		}

		public ETokenResult GetToken()
		{
			m_number = 0;

			for (int i = 0; i < m_string.Length; i++)
			{
				m_string[i] = ' ';
			}

			char ch;

			while (true)
			{
				if ((ch = GetChar()) == '@')
				{
					return ETokenResult.TOKEN_END;
				}

				if (Char.IsWhiteSpace(ch) || ch == '\t')
				{
					continue;
				}

				if ((ch = CheckComment(ch)) == '@')
				{
					return ETokenResult.TOKEN_END;
				}
				else if (ch != '\n')
				{
					break;
				}
			}

			if (ch == '-' || ch == '+' || ch == '.' || ch == '*' || Char.IsDigit(ch))
			{
				return GetTokenNumber(ch);
			}

			if (ch == '"')
			{
				return GetTokenString();
			}

			return GetTokenCommon(ch);
		}

		ETokenResult GetTokenNumber(char ch)
		{
			int count = 0;

			UnGetChar(ch);

			while ((ch = GetChar()) != '@' && (ch == '-' || ch == '+' || ch == ',' || ch == '*' || Char.IsDigit(ch)))
			{
				m_string[count++] = ch;
			}

			string checkString = new string(m_string).Trim();

			if (checkString.Equals("*"))
			{
				m_number = -1;
			}
			else
			{
				m_number = (float)MyAtof(m_string);
			}

			m_string[count] = ' ';

			return ETokenResult.TOKEN_NUMBER;
		}

		ETokenResult GetTokenString()
		{
			char ch;

			int count = 0;

			while ((ch = GetChar()) != '@' && ch != '"')
			{
				m_string[count++] = ch;
			}

			if (ch != '"')
			{
				UnGetChar(ch);
			}

			m_string[count] = ' ';

			return ETokenResult.TOKEN_STRING;
		}

		ETokenResult GetTokenCommon(char ch)
		{
			if (!Char.IsLetter(ch))
			{
				return ETokenResult.TOKEN_ERROR;
			}

			int count = 0;

			m_string[count++] = ch;

			while ((ch = GetChar()) != '@' && (ch == ',' || ch == '_' || Char.IsLetter(ch) || Char.IsDigit(ch)))
			{
				m_string[count++] = ch;
			}

			UnGetChar(ch);

			m_string[count] = '\0';

			return ETokenResult.TOKEN_STRING;
		}

		private void SetLastError(int error)
		{
			switch (error)
			{
				case 0:
				{
					m_LastError = string.Format(MEM_SCRIPT_ERROR_CODE0, m_path);

					break;
				}

				case 1:
				{
					m_LastError = string.Format(MEM_SCRIPT_ERROR_CODE1, m_path);

					break;
				}

				case 2:
				{
					m_LastError = string.Format(MEM_SCRIPT_ERROR_CODE2, m_path);

					break;
				}

				case 3:
				{
					m_LastError = string.Format(MEM_SCRIPT_ERROR_CODE3, m_path);

					break;
				}

				case 4:
				{
					m_LastError = string.Format(MEM_SCRIPT_ERROR_CODE4, m_path);

					break;
				}

				default:
				{
					m_LastError = string.Format(MEM_SCRIPT_ERROR_CODEX, m_path, error);

					break;
				}
			}
		}

		public string GetLastError()
		{
			return m_LastError;
		}

		public int GetNumber()
		{
			return (int)m_number;
		}

		public int GetAsNumber()
		{
			GetToken();

			return (int)m_number;
		}

		public float GetFloatNumber()
		{
			return m_number;
		}

		public float GetAsFloatNumber()
		{
			GetToken();

			return m_number;
		}

		public string GetString()
		{
			return new string(m_string).Trim();
		}

		public string GetAsString()
		{
			GetToken();

			return new string(m_string).Trim();
		}

		private bool testChar(char c)
		{
			return (c == ',' || c == '-' || c == '+' || char.IsDigit(c));
		}

		private float MyAtof(char[] myString)
		{
			string thisString = new string(myString);

			thisString = new string(thisString.Trim().TakeWhile(testChar).ToArray());

			if (thisString.Length > 0)
			{
				float rvl;

				if (float.TryParse(thisString, out rvl))
				{
					return rvl;
				}
			}

			return 0;
		}
	}
}