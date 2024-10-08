# from openai import OpenAI
# import sys

# client=OpenAI(api_key=sys.argv[1]) #modification starts.

# if __name__ == '__main__':
#     # Check if the script is called with a specific argument
#     if len(sys.argv) == 2 and sys.argv[1] == 'call':
#         print("Script was called! Perform the desired action here.")
#         sys.exit(0)
    
#     # The script continues with the original functionality

#     # Check if the correct number of command line arguments is provided
#     if len(sys.argv) < 5:
#         print("Usage: python3 api_call.py API_KEY paragraph_0.txt paragraph_1.txt paragraph_2.txt query.txt")
#         sys.exit(1)

#     # Read the API key from the command line
    
#     num_paragraphs=sys.argv[2]
#     OpenAI.api_key = sys.argv[1]

#     # Read the paragraphs from the files
#     paragraphs = []



#     for i in range(2, len(sys.argv)):
#         with open(sys.argv[i], 'r') as f:
#             paragraphs.append(f.read())


#     text_file=sys.argv[3]
#     with open(text_file,'r') as f:
#         query=f.read()
#         paragraphs.append(query)
#         paragraphs.append('\n')

#     # convert paragraphs to a single string
#     paragraphs = '\n'.join(paragraphs)

#     query = {
#         "role": "user", "content": paragraphs
#     }

#     chat = client.chat.completions.create(
#         model="gpt-3.5-turbo",
#         messages=[query]
#     )

#     reply = chat.choices[0].message.content
#     print(reply)


from openai import OpenAI
import sys

client=OpenAI(api_key=sys.argv[1])


if __name__ == '__main__':
    
    # python3 <filename> API_KEY num_paragraphs query.txt
    if len(sys.argv) < 4:
        print("Usage: python3 api_call.py API_KEY num_paragraphs query.txt")
        sys.exit(1)

    # Read the API key from the command line

    OpenAI.api_key = sys.argv[1]
    num_paragraphs = int(sys.argv[2])

    # Read the paragraphs from the files
    paragraphs = []

    for i in range(num_paragraphs):
        filename = 'paragraph_' + str(i) + '.txt'
        with open(filename, 'r') as f:
            paragraphs.append(f.read())
            paragraphs.append('\n')
    
    # add query
    query_file = sys.argv[3]
    with open(query_file, 'r') as f:
        query = f.read()
        paragraphs.append(query)
        paragraphs.append('\n')

    # convert paragraphs to a single string
    paragraphs = '\n'.join(paragraphs)

    # print(paragraphs)

    query = {
        "role": "user", "content": paragraphs
    }

    chat = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[query]
    )

    reply = chat.choices[0].message.content
    #print(query)
    print(reply)