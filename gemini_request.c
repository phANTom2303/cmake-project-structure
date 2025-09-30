#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A struct to hold the response from the server
struct MemoryStruct {
    char *memory;
    size_t size;
};

// This callback function gets called by libcurl as soon as there is data
// received
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    // Reallocate memory for the response buffer
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(void) {
    CURL *curl_handle;
    CURLcode res;

    // Initialize the struct that will hold our response
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  // will be grown as needed by the callback
    chunk.size = 0;            // no data at this point

    // Get API Key from environment variable for security
    const char *api_key = getenv("GEMINI_API_KEY");
    if (!api_key) {
        fprintf(stderr,
                "Error: GEMINI_API_KEY environment variable not set.\n");
        return 1;
    }

    // Construct the full URL with the API key
    char url[256];
    snprintf(url, sizeof(url),
             "https://generativelanguage.googleapis.com/v1beta/models/"
             "gemini-2.5-flash-lite:generateContent?key=%s",
             api_key);

    // The JSON data to send in the POST request
    const char *post_data =
        "{\"contents\":[{\"parts\":[{\"text\":\"Explain how AI works in a few "
        "words\"}]}]}";

    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_ALL);

    // Initialize a curl easy handle
    curl_handle = curl_easy_init();
    if (curl_handle) {
        // Set the required headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Set the curl options
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, post_data);

        // Set the callback function to handle the response
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,
                         WriteMemoryCallback);
        // Pass our 'chunk' struct to the callback function
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

        // Set a user-agent
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl_handle);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        } else {
            // The request was successful, print the response
            printf("Response from server:\n%s\n", chunk.memory);
        }

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl_handle);
        free(chunk.memory);
    }

    // Global cleanup
    curl_global_cleanup();

    return 0;
}