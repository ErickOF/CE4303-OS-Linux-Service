import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Config } from 'src/app/config/config';
import { Observable } from 'rxjs';
import { Image } from 'src/app/models/image';


@Injectable({
  providedIn: 'root'
})
export class ImageManipulationService {

  constructor(private http: HttpClient) { }

  private _base64ToArrayBuffer(data) {
    const base64Data = data.split(',')[1];
    const contentType = 'application/x-www-form-urlencoded';
    const sliceSize = 1024;
    let byteCharacters = atob(base64Data);
    let bytesLength = byteCharacters.length;
    let slicesCount = Math.ceil(bytesLength / sliceSize);
    let byteArrays = new Array(slicesCount);

    for (let sliceIndex = 0; sliceIndex < slicesCount; ++sliceIndex) {
        let begin = sliceIndex * sliceSize;
        let end = Math.min(begin + sliceSize, bytesLength);

        let bytes = new Array(end - begin);
        for (let offset = begin, i = 0; offset < end; ++i, ++offset) {
            bytes[i] = byteCharacters[offset].charCodeAt(0);
        }
        byteArrays[sliceIndex] = new Uint8Array(bytes);
    }

    return new Blob(byteArrays, { type: contentType });
  }

  public filterImage(img: Image): Observable<any> {
    const headers = new HttpHeaders();
    headers.append('Content-Type', 'application/x-www-form-urlencoded');
    headers.append('Access-Control-Allow-Origin', '*');
    headers.append('Access-Control-Allow-Headers', '*');
    headers.append('Cache-Control', 'no-cache');
    headers.append('Filename', img.name);

    return this.http.post(Config.HOST, this._base64ToArrayBuffer(img.data), { headers, responseType: 'text' });
  }
}
