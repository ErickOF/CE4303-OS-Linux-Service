import { Component } from '@angular/core';
import { AlertController } from '@ionic/angular';
import { Camera, CameraOptions } from '@ionic-native/camera/ngx';
import { Image } from '../models/image';
import { ImageManipulationService } from '../services/image-manipulation/image-manipulation.service';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {
  public source: string;
  public img: Image;
  public images = [];

  constructor(
    public alertController: AlertController,
    public camera: Camera,
    public imgManipulationService: ImageManipulationService
  ) {
    this.source = 'https://pngimage.net/wp-content/uploads/2018/06/lena-png-3.png';
  }

  async showAlert(header: string, subHeader: string, msg: string) {
    const alert = await this.alertController.create({
      cssClass: 'my-custom-class',
      header: header,
      subHeader: subHeader,
      message: msg,
      buttons: ['OK']
    });

    await alert.present();
  }

  public openGallery() {
    const options: CameraOptions = {
      quality: 100,
      destinationType: this.camera.DestinationType.DATA_URL,
      encodingType: this.camera.EncodingType.PNG,
      mediaType: this.camera.MediaType.PICTURE,
      sourceType: this.camera.PictureSourceType.PHOTOLIBRARY
    }

    this.camera.getPicture(options).then((imgData) => {
      const base64 = 'data:image/jpeg;base64,' + imgData;
      this.img = {
        name: 'Lena.png',
        data: base64,
        source: base64
      }
    });
  }

  public onProcessImage(): void {
    if (this.img) {
      const response = this.imgManipulationService.filterImage(this.img);

      response.subscribe((text) => {
        this.showAlert('Image was sent!', '', 'Image was processing correctly.');
      }, (error) => {
        this.showAlert('Error!', '', 'Image wasn\'t sent');
      });
    } else {
      this.showAlert('Image was\'nt selected!', '', 'You must select an image.');
    }
  }

}
